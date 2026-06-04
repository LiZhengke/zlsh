.code32
.section .text.entry, "ax"
.global _start
.type _start, @function

.extern main

_start:
    call main
    /* Propagate main's return value to sys_exit. */
    mov %eax, %ebx
    mov $3, %eax   # sys_exit
    int $0x30
.hang:
    jmp .hang

.section .note.GNU-stack,"",@progbits
