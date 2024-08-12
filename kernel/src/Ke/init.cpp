//
// Created by Piotr on 21.07.2024.
//

#include <Mm/virtual_memory_manager.hpp>
#include <Gfx/fonts/render_text.hpp>
#include <Mm/simple_allocator.hpp>
#include <Mm/memory_manager.hpp>
#include <Rtl/conversion.hpp>
#include <Io/framebuffer.hpp>
#include <Rtl/init_array.hpp>
#include <Hal/Irq/irq.hpp>
#include <Fw/Acpi/acpi.hpp>
#include <Hal/Pci/pci.hpp>
#include <Hal/Smp/smp.hpp>
#include <Hal/Int/idt.hpp>
#include <Hal/Mem/gdt.hpp>
#include <Rtl/memory.hpp>
#include <Std/string.hpp>
#include <Ke/debug.hpp>
#include <Io/cpu.hpp>

#include "cute.hpp"

#include <string>

limine_module_request module_request = {
        .id = LIMINE_MODULE_REQUEST,
        .revision = 0,
        .response = nullptr,
};

extern "C" void KeInitSystem()
{
    limine_framebuffer *framebuffer;
    IoQueryFramebuffer(&framebuffer, 0);
    if (framebuffer == nullptr) IoHaltProcessor();

    MmInitializeMemoryManager();
    DbgPrint("Past initializing memory manager.\n");

    MmVmInitialize();
    DbgPrint("Past initializing virtual memory manager.\n");

    RtlInitializeGlobalHeap();

    AbiCallCtors();
    DbgPrint("Called ABI constructors.\n");

    HalLoadGlobalDescriptorTable();
    DbgPrint("Loaded new GDT.\n");

    HalLoadInterruptDescriptorTable();
    DbgPrint("Loaded new IDT.\n");

    HalEnableInterrupts();
    DbgPrint("Enabled interrupts.\n");

    FwInitializeACPI();
    DbgPrint("Initialized ACPI subsystem.\n");

    HalIrqInitializeAdvancedIRQs();
    DbgPrint("Initialized APIC.\n");

    HalSmpInitializeLogicalCores();
    DbgPrint("Initialized other cores.\n");

    if (module_request.response == nullptr)
    {
        DbgPrint("Modules request response is null.\n");
        IoHaltProcessor();
    }

    limine_file *font_file = nullptr;
    for (size_t mod_idx = 0; module_request.response->module_count > mod_idx; mod_idx++)
    {
        auto module = module_request.response->modules[mod_idx];

        if (strcmp(module->cmdline, "0xProtoNerdFontMono-Regular.ttf") == 0)
        {
            font_file = module;
            break;
        }
    }

    if (font_file == nullptr)
    {
        DbgPrint("Font has not been found.\n");
        IoHaltProcessor();
    }

    GfxTextInitializeWithTTFFont(font_file->address, 20);

    auto ecount = MmRetrieveMemoryMapEntryCount();
    auto entries = MmRetrieveMemoryMap();

    uint64_t usable_memory = 0;

    for (size_t i = 0; ecount > i; i++)
    {
        auto entry = entries[i];

        // auto type_name = MmInterpretMemoryMapEntryTypeAsWideString(entry->type);

        // GfxTextWprintf(L"%d. %llx -> %llx (%f %s)\t[%ls]\n", i, entry->base, entry->base + entry->length, size, name, type_name);

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            usable_memory += entry->length;
        }
    }

    auto size = RtlConvertDataSize(usable_memory, BYTE, MEBIBYTE);
    auto name = RtlInterpretDataSizeAsStringShortForm(MEBIBYTE);
    GfxTextWprintf(L"Usable memory: %f [%s]\n", size, name);

    HalPciInitializePCIExpress();

    int spaces = 100;
    int ls = 0;
    int rs = 20;
    while (1)
    {
        if (rs == 0)
        {
            rs = spaces;
            ls = 0;
        }

        for (size_t i = 0; ls > i; i++)
            GfxTextWprintf(L" ");

        GfxTextWprintf(L".\n");

        ls++;
        rs--;
    }

    asm volatile ("int $0x0");

    while (1)
    {
        asm volatile ("nop");
    }

    //IoHaltProcessor();
}