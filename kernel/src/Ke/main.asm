section .data
fs_base_structure:
    times 4096 db 0

section .text

extern KeInitSystem
EFER_MSR_INDEX equ 0xC0000080
global _start
_start:
    lea rax, [rel fs_base_structure]
    mov ecx, 0xC0000100
    mov rbx, rax
    shr rax, 32
    mov edx, eax
    mov eax, ebx
    wrmsr

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