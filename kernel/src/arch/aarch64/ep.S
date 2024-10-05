.section .text
.global _start
.extern KernelMain
_start:
    and x29, x29, 0xfffffffffffffff0  // Align stack pointer (x29 is the frame pointer)
    b KernelMain