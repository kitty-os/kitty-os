//
// Created by Piotr on 21.07.2024.
//

#include <Mm/virtual_memory_manager.hpp>
#include <CxxRuntime/init_array.hpp>
#include <Mm/simple_allocator.hpp>
#include <Mm/memory_manager.hpp>
#include <Exe/PE32P/pe32p.hpp>
#include <Io/framebuffer.hpp>
#include <Hal/Irq/apic.hpp>
#include <Fw/Acpi/acpi.hpp>
#include <Hal/Int/idt.hpp>
#include <Hal/Mem/gdt.hpp>
#include <Std/string.hpp>
#include <Ke/debug.hpp>
#include <Io/cpu.hpp>
#include <Gfx/2d.hpp>
#include <Io/msr.hpp>

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

limine_module_request module_request = {
        .id = LIMINE_MODULE_REQUEST,
        .revision = 0,
        .response = nullptr,
        .internal_module_count = 0,
        .internal_modules = nullptr,
};

void syscall_entry_point()
{
    DbgPrint("Syscall!\n");
    while (1);
}

extern "C" void KeInitSystem()
{
    limine_framebuffer* framebuffer;
    IoQueryFramebuffer(&framebuffer, 0);
    if (framebuffer == nullptr) IoHaltProcessor();

    MmInitializeMemoryManager();
    DbgPrint("Past initializing memory manager.\n");

    MmVmInitialize();
    DbgPrint("Past initializing virtual memory manager.\n");

    AbiCallCtors();
    DbgPrint("Called ABI constructors.\n");

    HalLoadGlobalDescriptorTable();
    DbgPrint("Loaded new GDT.\n");

    HalLoadInterruptDescriptorTable();
    DbgPrint("Loaded new IDT.\n");

    HalEnableInterrupts();
    DbgPrint("Enabled interrupts.\n");

    HalIrqInitializeAPIC();
    DbgPrint("Enabled APIC.\n");

    FwInitializeACPI();
    DbgPrint("Initialized ACPI subsystem.\n");

    char madt_table_name[4] = {'A', 'P', 'I', 'C'};
    auto madt_table_address = FwAcpiQueryTable(madt_table_name);
    if (madt_table_address == nullptr)
    {
        DbgPrint("No MADT table has been found.\n");
    }

    DbgPrint("MADT table address: ");
    DbgPrintHexadecimal((uint64_t) madt_table_address);
    DbgPrintChar('\n');

    limine_file* driver_file = nullptr;

    for (size_t i = 0; module_request.response->module_count > i; i++)
    {
        auto module = module_request.response->modules[i];

        if (strcmp(module->cmdline, "HelloWorld.sys") == 0)
        {
            driver_file = module;
            break;
        }
    }

    if (driver_file == nullptr)
    {
        DbgPrint("Driver hasnt been found.\n");
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

    driver_information_block.image_base_address = (void*)va;
    //PE32PExeResolveImportTable(driver_file->address, &driver_information_block);

    uint64_t user_code_seg = 7*8;
    uint64_t user_data_seg = 8*8;
    uint64_t kernel_code_seg = 1*8;
    uint64_t star_value = ((user_code_seg << 48) | (user_data_seg << 32) | (kernel_code_seg << 16));


    wrmsr(0xC0000081, star_value);
    wrmsr(0xC0000082, (uint64_t)&syscall_entry_point);
    wrmsr(0xC0000084, 0x3d0);
    wrmsr(0xC0000100, 0);
    wrmsr(0xC0000101, 0);

    // Jump to the entry point
    asm volatile ("xchg %bx, %bx");
    jumpToEntryPoint(va, driver_information_block.optional_header);

    IoHaltProcessor();
}