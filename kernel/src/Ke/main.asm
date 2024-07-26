section .text

extern KeInitSystem

global _start
_start:
    mov rax, cr0
    and ax, 0xfffb
    or ax, 0x2
    mov cr0, rax

    mov rax, cr4
    or ax, 3 << 9
    mov cr4, rax

    call KeInitSystem

    cli
    hlt