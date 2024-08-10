extern "C"
{
#include <xed/xed-interface.h>
}

#include <Decompiler/decompiler.hpp>
#include <Mm/memory_manager.hpp>
#include <Hal/Irq/apic.hpp>
#include <Ke/debug.hpp>
#include <Io/msr.hpp>
#include <Io/cpu.hpp>
#include "idt.hpp"


extern IDTGateDescriptor idt_table[256];

void PrintInterruptFrame(const struct InterruptFrame *frame) {
    // ANSI escape codes for colors
    const char *color_reset = "\033[0m";
    const char *color_header = "\033[1;34m"; // Blue
    const char *color_value = "\033[1;32m";  // Green

    DbgPrintf("%sInterrupt Frame:%s\n", color_header, color_reset);
    DbgPrintf("  %sCR3             :%s 0x%llx\n", color_header, color_value, frame->cr3);
    DbgPrintf("  %sCS              :%s 0x%llx\n", color_header, color_value, frame->cs);
    DbgPrintf("  %sDS              :%s 0x%llx\n", color_header, color_value, frame->ds);
    DbgPrintf("  %sSS              :%s 0x%llx\n", color_header, color_value, frame->ss);
    DbgPrintf("  %sES              :%s 0x%llx\n", color_header, color_value, frame->es);
    DbgPrintf("  %sFS              :%s 0x%llx\n", color_header, color_value, frame->fs);
    DbgPrintf("  %sGS              :%s 0x%llx\n", color_header, color_value, frame->gs);
    DbgPrintf("  %sR15             :%s 0x%llx\n", color_header, color_value, frame->r15);
    DbgPrintf("  %sR14             :%s 0x%llx\n", color_header, color_value, frame->r14);
    DbgPrintf("  %sR13             :%s 0x%llx\n", color_header, color_value, frame->r13);
    DbgPrintf("  %sR12             :%s 0x%llx\n", color_header, color_value, frame->r12);
    DbgPrintf("  %sR11             :%s 0x%llx\n", color_header, color_value, frame->r11);
    DbgPrintf("  %sR10             :%s 0x%llx\n", color_header, color_value, frame->r10);
    DbgPrintf("  %sR9              :%s 0x%llx\n", color_header, color_value, frame->r9);
    DbgPrintf("  %sR8              :%s 0x%llx\n", color_header, color_value, frame->r8);
    DbgPrintf("  %sRDI             :%s 0x%llx\n", color_header, color_value, frame->rdi);
    DbgPrintf("  %sRSI             :%s 0x%llx\n", color_header, color_value, frame->rsi);
    DbgPrintf("  %sRBP             :%s 0x%llx\n", color_header, color_value, frame->rbp);
    DbgPrintf("  %sRSP             :%s 0x%llx\n", color_header, color_value, frame->rsp);
    DbgPrintf("  %sRDX             :%s 0x%llx\n", color_header, color_value, frame->rdx);
    DbgPrintf("  %sRCX             :%s 0x%llx\n", color_header, color_value, frame->rcx);
    DbgPrintf("  %sRBX             :%s 0x%llx\n", color_header, color_value, frame->rbx);
    DbgPrintf("  %sRAX             :%s 0x%llx\n", color_header, color_value, frame->rax);
    DbgPrintf("  %sInterrupt Number:%s 0x%llx\n", color_header, color_value, frame->interrupt_number);
    DbgPrintf("  %sError Code      :%s 0x%llx\n", color_header, color_value, frame->error_code);
    DbgPrintf("  %sRIP             :%s 0x%llx\n", color_header, color_value, frame->rip);
    DbgPrintf("  %sRFLAGS          :%s 0x%llx\n", color_header, color_value, frame->rflags);
    DbgPrintf("  %sOrig RSP        :%s 0x%llx\n", color_header, color_value, frame->orig_rsp);
    DbgPrintf("%s", color_reset); // Reset color
}

static inline uint64_t ReadPageFaultAddress()
{
    uint64_t cr2;
    asm volatile ("mov %%cr2, %0" : "=r" (cr2));
    return cr2;
}

// Function to interpret the page fault error code
void InterpretPageFaultErrorCode(uint64_t error_code)
{
    // Masks for the error code bits
    const uint64_t PRESENT_MASK = 0x1;
    const uint64_t WRITE_MASK = 0x2;
    const uint64_t USER_MASK = 0x4;
    const uint64_t RESERVED_WRITE_MASK = 0x8;
    const uint64_t INSTRUCTION_FETCH_MASK = 0x10;
    const uint64_t PROTECTION_KEY_MASK = 0x20;
    const uint64_t SHADOW_STACK_MASK = 0x40;
    const uint64_t SGX_MASK = 0x80;

    // Interpreting the error code
    bool present = (error_code & PRESENT_MASK) != 0;
    bool write = (error_code & WRITE_MASK) != 0;
    bool user = (error_code & USER_MASK) != 0;
    bool reserved_write = (error_code & RESERVED_WRITE_MASK) != 0;
    bool instruction_fetch = (error_code & INSTRUCTION_FETCH_MASK) != 0;
    bool protection_key = (error_code & PROTECTION_KEY_MASK) != 0;
    bool shadow_stack = (error_code & SHADOW_STACK_MASK) != 0;
    bool sgx = (error_code & SGX_MASK) != 0;

    // Print the interpreted results
    DbgPrintf("Page Fault Error Code Interpretation:\n");
    DbgPrintf("  Present            : %s\n", present ? "Yes" : "No");
    DbgPrintf("  Write              : %s\n", write ? "Yes" : "No");
    DbgPrintf("  User Mode          : %s\n", user ? "Yes" : "No");
    DbgPrintf("  Reserved Write     : %s\n", reserved_write ? "Yes" : "No");
    DbgPrintf("  Instruction Fetch  : %s\n", instruction_fetch ? "Yes" : "No");
    DbgPrintf("  Protection Key     : %s\n", protection_key ? "Yes" : "No");
    DbgPrintf("  Shadow Stack       : %s\n", shadow_stack ? "Yes" : "No");
    DbgPrintf("  SGX                : %s\n", sgx ? "Yes" : "No");
}

extern "C" void HalInterruptHandler(InterruptFrame* frame)
{
    if (frame->interrupt_number < 32)
    {
        xed_decoded_inst_t xedd;
        DecompilerDecompileAtRIPRange(&xedd, (const uint8_t*)frame->rip, 32, 32);
        PrintInterruptFrame(frame);

        if (frame->interrupt_number == 0xe)
        {
            auto page_fault_address = ReadPageFaultAddress();
            DbgPrintf("%llx\n", page_fault_address);
            InterpretPageFaultErrorCode(frame->error_code);
        }

        IoHaltProcessor();
    }

    if (frame->interrupt_number == 0x20)
    {
        uint64_t apic_base = 0;
        rdmsr(IA32_APIC_BASE_MSR, &apic_base);
        apic_base &= ~(0xfff);
        apic_base += MmGetHigherHalfDirectMemoryOffset();
        HalIrqApicEndOfInterrupt(apic_base);
    }
}

extern "C" void HalFlushInterruptDescriptorTableImpl(IDTRegister* idt_register);

void HalLoadInterruptDescriptorTable()
{
    IDTRegister idt_register = {
            .limit = sizeof(idt_table) - 1,
            .address = (uint64_t)&idt_table
    };

    HalFlushInterruptDescriptorTableImpl(&idt_register);
}
