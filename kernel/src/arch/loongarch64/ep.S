.section .text
.global _start
.extern KernelMain
_start:
    li.w $r12, 0
    li.w $r13, 15
    sub.d $r12, $r12, $r13
    and $r3, $r3, $r12
    b KernelMain