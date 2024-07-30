//
// Created by Piotr on 28.07.2024.
//

#include <Mm/memory_manager.hpp>
#include <Io/msr.hpp>
#include "apic.hpp"

void HalIrqWriteToAPICRegister(uint32_t base_address, uint32_t offset, uint32_t value)
{
    volatile uint32_t* address = (volatile uint32_t*)(base_address + offset + MmGetHigherHalfDirectMemoryOffset());
    *address = value;
}

uint32_t HalIrqReadFromAPICRegister(uint32_t base_address, uint32_t offset)
{
    volatile uint32_t* address = (volatile uint32_t*)(base_address + offset + MmGetHigherHalfDirectMemoryOffset());
    return *address;
}

void HalIrqSetAPICBase(uintptr_t apic)
{
    uint32_t edx = 0;
    uint32_t eax = (apic & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;

    edx = (apic >> 32) & 0x0f;

    wrmsr(IA32_APIC_BASE_MSR, (uint64_t)eax << 32 | edx);
}

uintptr_t HalIrqGetAPICBase()
{
    uint64_t msr_value;

    rdmsr(IA32_APIC_BASE_MSR, &msr_value);

    return (uintptr_t)(msr_value & 0xfffff000);
}

void HalIrqInitializeAPIC()
{
    auto base = HalIrqGetAPICBase();
    HalIrqSetAPICBase(HalIrqGetAPICBase());
    HalIrqWriteToAPICRegister(base, APIC_REG_SPURIOUS_INT_VEC, HalIrqReadFromAPICRegister(base, APIC_REG_SPURIOUS_INT_VEC) | 0x100); // Enable
}