section .text

extern KeInitSystem
EFER_MSR_INDEX equ 0xC0000080
global _start
_start:
    and rsp, 0xfffffffffffff000

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