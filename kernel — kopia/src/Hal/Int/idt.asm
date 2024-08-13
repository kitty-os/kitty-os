bits 64

section .text

extern HalInterruptHandler
global HalFlushInterruptDescriptorTableImpl
HalFlushInterruptDescriptorTableImpl:
    lidt [rdi]
    ret

%macro ISR_NOERRORCODE 1
    global InterruptHandler%1:
    InterruptHandler%1:
        push qword 0 ; dummy error code
        push qword %1 ; interrupt number
        jmp HalISRCommon
%endmacro

%macro ISR_ERRORCODE 1
    global InterruptHandler%1:
    InterruptHandler%1:
        push qword %1 ; interrupt number
        jmp HalISRCommon
%endmacro

%include 'isr.inc'

global HalISRCommon
HalISRCommon:
    push rax
    push rbx
    push rcx
    push rdx
    push rsp
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    xor rax, rax

    mov ax, ds
    push rax
    mov ax, es
    push rax
    mov ax, fs
    push rax
    mov ax, gs
    push rax

    mov rax, cr3
    push rax

    cld

    ; Push the address of the structure onto the stack
    lea rdi, [rsp]   ; Address of the saved registers
    call HalInterruptHandler

    pop rax
    mov cr3, rax

    xor rax, rax
    pop rax
    mov gs, ax
    pop rax
    mov fs, ax
    pop rax
    mov es, ax
    pop rax
    mov ds, ax

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rsp
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16
    iretq
