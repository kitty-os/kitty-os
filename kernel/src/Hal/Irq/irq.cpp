//
// Created by Piotr on 04.08.2024.
//

#include <Hal/Irq/ioapic.hpp>
#include <Fw/Acpi/acpi.hpp>
#include <Hal/Irq/apic.hpp>
#include <Ke/debug.hpp>
#include "irq.hpp"

void HalIrqInitializeAdvancedIRQs()
{
    HalIrqInitializeLAPIC();
    FwAcpiEnumerateMADTEntries([](void* madt_entry_pointer){
        auto entry_header = (MADTEntry*) madt_entry_pointer;
        auto ioapic_iso = (MADTIOAPICISO*) madt_entry_pointer;
        auto lapic = (MADTLAPIC*) madt_entry_pointer;

        switch (entry_header->entry_type)
        {
            case MADT_ENTRY_TYPE_IOAPIC_INTERRUPT_SOURCE_OVERRIDE:
                DbgPrintf("ISA %d -> GSI %d\n", ioapic_iso->irq_source, ioapic_iso->gsi);
                break;
            case MADT_ENTRY_TYPE_IOAPIC:
                HalIrqInitializeIOAPIC(((MADTIOAPIC*)madt_entry_pointer)->ioapic_address);
                break;
            default:
                break;
        }
    });
}