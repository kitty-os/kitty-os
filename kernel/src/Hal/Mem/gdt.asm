bits 64
section .text

global HalLoadGlobalDescriptorTableImpl
HalLoadGlobalDescriptorTableImpl:
    lgdt [rdi]

    ; Reload segments to kernel cs and ds.
.reloadSegments:
    push 0x08
    lea rax, [rel .reload_CS]
    push rax
    retfq
.reload_CS:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    ret