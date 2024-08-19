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
#include <Rtl/string.hpp>
#include <Ke/debug.hpp>
#include <Io/cpu.hpp>

//#include <Io/Fs/ext2/ext2.hpp>
#include <Io/SIs/MSI/msi.hpp>
#include <Io/Vfs/vfs.hpp>

#include "cute.hpp"


limine_module_request module_request = {
        .id = LIMINE_MODULE_REQUEST,
        .revision = 0,
        .response = nullptr,
};

limine_file* KeRequestModule(const char* cmdline)
{
    limine_file *file = nullptr;
    for (size_t mod_idx = 0; module_request.response->module_count > mod_idx; mod_idx++)
    {
        auto module = module_request.response->modules[mod_idx];

        if (strcmp(module->cmdline, cmdline) == 0)
        {
            file = module;
            return file;
        }
    }

    if (file == nullptr)
    {
        DbgPrintf("File %s not found.\n", cmdline);
    }

    return nullptr;
}

extern "C" void KeInitSystem()
{
    limine_framebuffer *framebuffer;
    IoQueryFramebuffer(&framebuffer, 0);
    if (framebuffer == nullptr) IoHaltProcessor();

    MmInitializeMemoryManager();
    DbgPrint("Past initializing memory manager.\n");

    auto curr_usage = MmGetUsableMemory();

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

    //HalSmpInitializeLogicalCores();
    //DbgPrint("Initialized other cores.\n");

    if (module_request.response == nullptr)
    {
        DbgPrint("Modules request response is null.\n");
        IoHaltProcessor();
    }

    limine_file *font_file = KeRequestModule("JetBrainsMonoNerdFontMono-Regular.ttf");
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
    GfxTextWprintf("Usable memory: %f [%s]\n", size, name);

    auto iso9660 = KeRequestModule("iso9660.iso");
    auto exfat = KeRequestModule("exfat.img");
    auto fat32 = KeRequestModule("fat32.img");
    auto fat16 = KeRequestModule("fat16.img");
    auto fat12 = KeRequestModule("fat12.img");
    auto ext2 = KeRequestModule("ext2.img");
    auto ext3 = KeRequestModule("ext3.img");
    auto ext4 = KeRequestModule("ext4.img");
    auto ntfs = KeRequestModule("ntfs.img");
    auto btrfs = KeRequestModule("btrfs.img");
    auto vfat = KeRequestModule("vfat.img");

    VFS vfs;
    MemoryStorageInterface msi0(0, ext2->address, ext2->size);
    //Ext2 ext2_0(msi0);
    //bool issys = ext2_0.IsFilesystem(msi0);
    //if (!issys)
    //{
    //    DbgPrintf("This is not a EXT4 filesystem.\n");
    //}
    //vfs.MountFilesystem("D:\\", std::make_unique<Ext2>(ext2_0));


    HalPciInitializePCIExpress();

    auto overall = (double) MmGetOverallMemory();
    auto final_usage = MmGetUsableMemory();
    auto delta = (double)(curr_usage - final_usage);
    GfxTextWprintf("Overall mem: %f (MB)\n", overall / 1024 / 1024);
    GfxTextWprintf("Used mem: %f (MB)\n", delta / 1024 / 1024);
    GfxTextWprintf("Usage: %f%%\n", final_usage / overall);

    HalPciEnumerateBus([](const PCIDevice& dev){
        DbgPrintf("%04hX:%04hX\n", dev.identifier.vendor_id, dev.identifier.device_id);
    });


    //IoHaltProcessor();
}