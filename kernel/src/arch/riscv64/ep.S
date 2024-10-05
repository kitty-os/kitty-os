.section .text
.global _start
_start:
    and sp, sp, 0xfffffffffffffff0    // Align stack pointer (sp is the stack pointer)
    csrci mstatus, 0x8                // Clear the global interrupt enable bit
    wfi                               // Wait for interrupt