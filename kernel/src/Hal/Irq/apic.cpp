//
// Created by Piotr on 04.08.2024.
//

#include <Mm/memory_manager.hpp>
#include <Ke/debug.hpp>
#include <Io/msr.hpp>
#include "apic.hpp"

void HalIrqApicWrite(uint64_t base, uint64_t offset, uint32_t value)
{
    volatile uint32_t* addr = (volatile uint32_t*) (base + offset);
    *addr = value;
}

uint32_t HalIrqApicRead(uint64_t base, uint64_t offset)
{
    volatile uint32_t* addr = (volatile uint32_t*) (base + offset);
    return *addr;
}

void HalIrqApicEndOfInterrupt(uint64_t base)
{
    HalIrqApicWrite(base, LAPIC_EOI_REGISTER, 0);
}

void HalIrqInitializeLAPIC()
{
    uint64_t apic_base = 0;
    rdmsr(IA32_APIC_BASE_MSR, &apic_base);
    apic_base &= ~(0xfff);
    apic_base += MmGetHigherHalfDirectMemoryOffset();

    HalIrqApicWrite(apic_base, LAPIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER, 0x1FF);

    HalIrqApicWrite(apic_base, LAPIC_TASK_PRIORITY_REGISTER, 0); // Allow them all.
}