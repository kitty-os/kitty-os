//
// Created by Piotr on 04.08.2024.
//

#include <Hal/Irq/ioapic.hpp>
#include <Fw/Acpi/acpi.hpp>
#include <Hal/Irq/apic.hpp>
#include <Hal/Smp/smp.hpp>
#include <Rtl/string.hpp>
#include <unordered_map>
#include <Ke/debug.hpp>
#include <limits>
#include "irq.hpp"

// a vector of pair containing an gsi base and base address to ioapic.
std::vector<MADTIOAPIC*> ioapics;
std::vector<MADTIOAPICISO*> ioapic_isos;

struct IDTVectorBitmap
{
    uint8_t bitmap[32];
};

using ProcessorIndex = size_t;
std::unordered_map<ProcessorIndex, IDTVectorBitmap> idt_bitmaps;

int FindFreeVector(const IDTVectorBitmap& bitmap)
{
    for (int i = 0; i < 256; ++i)
    {
        if (!(bitmap.bitmap[i / 8] & (1 << (i % 8))))
        {
            return i;
        }
    }
    return -1; // No free vector found
}
void SetVectorAllocated(IDTVectorBitmap& bitmap, int vector)
{
    bitmap.bitmap[vector / 8] |= (1 << (vector % 8));
}

int HalIrqAllocateIDTVectorOnCPU(ProcessorIndex cpu)
{
    auto& bitmap = idt_bitmaps[cpu];
    int vector = FindFreeVector(bitmap);
    if (vector != -1)
    {
        SetVectorAllocated(bitmap, vector);
    }
    return vector;
}
int HalIrqAllocateIDTVector()
{
    ProcessorIndex best_cpu = 0;
    int min_allocated = std::numeric_limits<int>::max();
    int best_vector = -1;

    for (auto& [cpu, bitmap] : idt_bitmaps)
    {
        int allocated = 0;
        for (int i = 0; i < 256; ++i)
        {
            if (bitmap.bitmap[i / 8] & (1 << (i % 8)))
            {
                ++allocated;
            }
        }
        if (allocated < min_allocated)
        {
            int vector = FindFreeVector(bitmap);
            if (vector != -1)
            {
                min_allocated = allocated;
                best_cpu = cpu;
                best_vector = vector;
            }
        }
    }

    if (best_vector != -1)
    {
        SetVectorAllocated(idt_bitmaps[best_cpu], best_vector);
    }

    return best_vector;
}

int HalIrqTranslateISAIRQToGSI(int irq)
{
    for (const auto& iso : ioapic_isos)
    {
        if (iso->irq_source == irq)
            return iso->gsi;
    }
    return irq; // Assuming that this ISA IRQ is identity mapped to a GSI.
}


bool HalIrqRegisterISAIRQSpecificCPU(int irq, std::function<void(InterruptFrame* _If)>)
{

}

bool HalIrqRegisterGSIIRQSpecificCPU(int gsi, std::function<void(InterruptFrame* _If)>)
{

}

void HalIrqHandleOtherInterrupts(InterruptFrame* frame)
{

}

void HalIrqInitializeAdvancedIRQs()
{
    auto cpu_count = HalSmpGetProcessorCount();
    for (size_t idx = 0; cpu_count > idx; idx++)
    {
        idt_bitmaps[idx] = {{}};
        memset(idt_bitmaps[idx].bitmap, 0, 32);

        // Mask out the exception handlers.
        idt_bitmaps[idx].bitmap[0] = 0xff;
        idt_bitmaps[idx].bitmap[1] = 0xff;
        idt_bitmaps[idx].bitmap[2] = 0xff;
        idt_bitmaps[idx].bitmap[3] = 0xff;
    }

    HalIrqInitializeLAPIC();
    FwAcpiEnumerateMADTEntries([](void* madt_entry_pointer){
        auto entry_header = (MADTEntry*) madt_entry_pointer;
        auto ioapic_iso = (MADTIOAPICISO*) madt_entry_pointer;
        auto lapic = (MADTLAPIC*) madt_entry_pointer;
        auto ioapic = (MADTIOAPIC*)madt_entry_pointer;

        switch (entry_header->entry_type)
        {
            case MADT_ENTRY_TYPE_IOAPIC_INTERRUPT_SOURCE_OVERRIDE:
                DbgPrintf("ISA %d -> GSI %d\n", ioapic_iso->irq_source, ioapic_iso->gsi);
                break;
            case MADT_ENTRY_TYPE_IOAPIC:
                ioapics.push_back(ioapic);
                HalIrqInitializeIOAPIC(((MADTIOAPIC*)madt_entry_pointer)->ioapic_address);
                break;
            default:
                break;
        }
    });
}