#include <debug.hpp>
#include <hal/msr.hpp>
#include <us/syscalls.hpp>

void initialize_syscall(uint64_t syscall_handler, uint32_t cs_selector, uint32_t ss_selector, uint32_t user_cs_selector,
                        uint32_t user_ss_selector, uint64_t rflags_mask)
{
    // Enable SYSCALL/SYSRET in IA32_EFER
    uint64_t efer = rdmsr(MSR_IA32_EFER);
    efer |= EFER_SCE; // Enable System Call Extensions
    wrmsr(MSR_IA32_EFER, efer);

    // Set IA32_LSTAR to the syscall handler entry point
    wrmsr(MSR_IA32_LSTAR, syscall_handler);

    // Configure IA32_STAR: user selectors in bits 63:48, kernel selectors in bits 47:32
    uint64_t star = ((uint64_t)user_cs_selector << 48) |
                    ((uint64_t)user_ss_selector << 32) | // User SS selector should be in bits 47:32
                    ((uint64_t)cs_selector << 16) |      // Kernel CS selector in bits 31:16
                    ((uint64_t)ss_selector);             // Kernel SS selector in bits 15:0
    wrmsr(MSR_IA32_STAR, star);

    // Set IA32_FMASK to mask specific RFLAGS bits
    wrmsr(MSR_IA32_FMASK, rflags_mask);
}

extern "C" void syscall_proper_handler(system_call_frame *frame)
{
    debug_printf("kocham piwo.\n");

    // Printing the register values, 4 registers per line
    debug_printf("Register Values:\n");

    debug_printf("R15: 0x%016lX  R14: 0x%016lX  R13: 0x%016lX  R12: 0x%016lX\n", frame->r15, frame->r14, frame->r13,
                 frame->r12);
    debug_printf("RFLAGS: 0x%016lX  R10: 0x%016lX  R9: 0x%016lX  R8: 0x%016lX\n", frame->rflags, frame->r10, frame->r9,
                 frame->r8);
    debug_printf("RDI: 0x%016lX  RSI: 0x%016lX  RBP: 0x%016lX  RSP: 0x%016lX\n", frame->rdi, frame->rsi, frame->rbp,
                 frame->rsp);
    debug_printf("RDX: 0x%016lX  NEXT_INS: 0x%016lX  RBX: 0x%016lX  RAX: 0x%016lX\n", frame->rdx, frame->next_isns,
                 frame->rbx, frame->rax);
}
