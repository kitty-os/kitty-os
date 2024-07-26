//
// Created by Piotr on 21.07.2024.
//

#include <Mm/virtual_memory_manager.hpp>
#include <Mm/simple_allocator.hpp>
#include <Mm/memory_manager.hpp>
#include <Exe/PE32P/pe32p.hpp>
#include <Io/framebuffer.hpp>
#include <Std/string.hpp>
#include <Ke/debug.hpp>
#include <Io/cpu.hpp>

limine_module_request module_request = {
        .id = LIMINE_MODULE_REQUEST,
        .revision = 0,
        .response = nullptr,
};

void jumpToEntryPoint(uintptr_t baseAddress, const IMAGE_OPTIONAL_HEADER64* optionalHeader) {
    // Calculate the address of the entry point
    uintptr_t entryPointAddress = baseAddress + optionalHeader->AddressOfEntryPoint;

    // Use inline assembly to jump to the entry point
    asm volatile (
            "jmp *%0"   // Use "jmp *" to specify an indirect jump to the address in %0
            :
            : "r" (entryPointAddress)  // Input operand: the address of the entry point
            : "memory"  // Clobbers: declare that this code does not modify any other registers
            );
}

extern "C" void KeInitSystem()
{
    limine_framebuffer* framebuffer;
    IoQueryFramebuffer(&framebuffer, 0);

    if (framebuffer == nullptr)
    {
        IoHaltProcessor();
    }

    MmInitializeMemoryManager();
    MmVmInitialize();

    if (module_request.response == nullptr)
    {
        DbgPrint("Failed to get modules.\n");
        IoHaltProcessor();
    }

    limine_file* driver_file = nullptr;
    for (size_t i = 0; module_request.response->module_count > i; i++)
    {
        auto this_module = module_request.response->modules[i];
        if (strcmp(this_module->cmdline, "HelloWorld.sys") == 0)
        {
            driver_file = this_module;
            break;
        }
    }

    if (driver_file == nullptr)
    {
        DbgPrint("Failed to find the HelloWorld.sys file.\n");
        IoHaltProcessor();
    }

    PE32PImageInformation driver_information_block;
    PE32PExeGetInformation(&driver_information_block, driver_file->address);

    if (!driver_information_block.is_valid)
    {
        DbgPrint("HelloWorld.sys is an invalid PE32+ DLL image.\n");
        IoHaltProcessor();
    }

    auto va = MmVmAllocateVirtualMemory(4096, driver_information_block.optional_header->SizeOfImage);
    auto pages_needed = (driver_information_block.optional_header->SizeOfImage + 4095) / 4096;

    for (size_t i = 0; pages_needed > i; i++)
    {
        MmMapPage(MmGetKernelPML4(), va + (i * 4096), (uint64_t) MmAllocatePage(), PROTECTION_KERNEL | PROTECTION_RW, MAP_PRESENT | MAP_GLOBAL);
    }

    // Load sections
    for (size_t section_index = 0; section_index < driver_information_block.file_header->NumberOfSections; section_index++)
    {
        auto section_entry = driver_information_block.section_header[section_index];

        DbgPrint("Loading section ");
        for (size_t i = 0; 8 > i; i++)
        {
            DbgPrintChar(section_entry.Name[i]);
        }
        DbgPrint("\n");

        auto size = section_entry.SizeOfRawData;
        auto virtual_address = section_entry.VirtualAddress;

        if (size > 0)
        {
            void* destination = (void*) (va + virtual_address);
            void* source = (void*)((uintptr_t)(driver_file->address) + section_entry.PointerToRawData);
            memcpy(destination, source, size);
        }
    }


    // Jump to the entry point
    asm volatile ("xchg %bx, %bx");
    jumpToEntryPoint(va, driver_information_block.optional_header);
    IoHaltProcessor();
}