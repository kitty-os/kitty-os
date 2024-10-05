.section .text
.global _start
.extern KernelMain
_start:
    andq $0xfffffffffffffff0, %rsp
    jmp KernelMain
    cli
    hlt
