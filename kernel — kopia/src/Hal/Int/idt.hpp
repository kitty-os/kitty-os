#pragma once

#include <cstdint>

struct IDTRegister
{
    uint16_t limit;     // sizeof(idt) - 1
    uint64_t address;   // Logical address
} __attribute__((packed));

struct IDTGateDescriptor
{
    uint16_t offset0;
    uint16_t segment_sel;
    uint8_t ist : 3;
    uint8_t reserved0 : 5;
    uint8_t gate_type : 4;
    uint8_t zero : 1;
    uint8_t dpl : 2;
    uint8_t present : 1;
    uint16_t offset1;
    uint32_t offset2;
    uint32_t reserved1;
} __attribute__((packed));

struct InterruptFrame
{
    uint64_t cr3, gs, fs, es, ds, r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rbp, rsp, rdx, rcx, rbx, rax, interrupt_number, error_code, rip, cs, rflags, orig_rsp, ss;
} __attribute__((packed));

#define IDT_GATE_TYPE_INTERRUPT 0xE
#define IDT_GATE_TYPE_TRAP_GATE 0xF

#define IDT_ENTRY(offset, segment, type, dpl, ist) {                     \
    (uint16_t)((offset) & 0xFFFF),                 /* offset0 */        \
    (segment),                                     /* segment_sel */    \
    (ist),                                         /* ist */            \
    0,                                             /* reserved0 */      \
    (type),                                        /* gate_type */      \
    0,                                             /* zero */           \
    (dpl),                                         /* dpl */            \
    1,                                             /* present */        \
    (uint16_t)(((offset) >> 16) & 0xFFFF),         /* offset1 */        \
    (uint32_t)(((offset) >> 32) & 0xFFFFFFFF),     /* offset2 */        \
    0                                              /* reserved1 */      \
}

void HalLoadInterruptDescriptorTable();
extern "C" void HalInterruptHandler(InterruptFrame* frame);

inline void HalEnableInterrupts()  { asm volatile ("sti"); }
inline void HalDisableInterrupts() { asm volatile ("cli"); }