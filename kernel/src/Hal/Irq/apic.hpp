#pragma once

#include <cstdint>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_ENABLE 0x800

#define APIC_REG(apic_base_address, offset) (*(volatile uint32_t *)(apic_base_address + (offset) + (MmGetHigherHalfDirectMemoryOffset())))

// APIC Register Offsets
#define APIC_REG_ID                0x20
#define APIC_REG_VERSION           0x30
#define APIC_REG_TASK_PRIORITY     0x80
#define APIC_REG_EOI               0xB0
#define APIC_REG_SPURIOUS_INT_VEC  0xF0
#define APIC_REG_LVT_TIMER         0x320
#define APIC_REG_TIMER_INIT_CNT    0x380
#define APIC_REG_TIMER_CURR_CNT    0x390
#define APIC_REG_TIMER_DIV_CONF    0x3E0

// APIC Interrupt Command Register Offsets
#define APIC_REG_ICR_LOW           0x300
#define APIC_REG_ICR_HIGH          0x310

// Spurious Interrupt Vector
#define SPURIOUS_VECTOR            0xFF

void HalIrqSetAPICBase(uintptr_t apic);
uintptr_t HalIrqGetAPICBase();
void HalIrqWriteToAPICRegister(uint32_t base_address, uint32_t offset, uint32_t value);
uint32_t HalIrqReadFromAPICRegister(uint32_t base_address, uint32_t offset);
void HalIrqInitializeAPIC();