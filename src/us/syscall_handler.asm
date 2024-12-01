bits 64

section .data
rsp_: dq 0

section .text

global syscall_handler
extern syscall_proper_handler
syscall_handler:
    ;; load stack there..?
    cli

    mov qword [rsp_], rsp    

    swapgs
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

    lea rdi, [rsp]
    call syscall_proper_handler

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
    swapgs
    
    mov rsp, qword [rsp_]

    sti
    o64 sysret