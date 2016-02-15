.intel_syntax noprefix

.global _asm_syscalls
.global _asm_exc_GP
.global _asm_exc_PF
.global _asm_schedule
.global _asm_int_1
.global _asm_int_2

.extern isr_PF_exc
.extern isr_GP_exc
.extern isr_schedule_int
.extern do_syscalls
.extern isr_default_int

.section .text

.macro SAVE_REGS
    pushad
    push ds
    push es
    push fs
    push gs
    push ebx
    mov bx,0x10
    mov ds,bx
    pop ebx
.endm

.macro RESTORE_REGS
    pop gs
    pop fs
    pop es
    pop ds
    popad
.endm

_asm_int_1:
    SAVE_REGS
    push 1
    call isr_default_int
    pop eax
    mov al,0x20
    out 0x20, al
    RESTORE_REGS
    iretd
    .size   _asm_int_1, . - _asm_int_1

_asm_int_2:
    SAVE_REGS
    push 2
    call isr_default_int
    pop eax
    mov al,0x20
    out 0x20, al
    RESTORE_REGS
    iretd
    .size   _asm_int_2, . - _asm_int_2

_asm_syscalls:
    SAVE_REGS
    push eax
    call do_syscalls
    pop eax
    cli
    sti
    RESTORE_REGS
    iretd
    .size   _asm_syscalls, . - _asm_syscalls

_asm_exc_GP:
    SAVE_REGS
    call isr_GP_exc
    RESTORE_REGS
    add esp,4
    iretd
    .size   _asm_exc_GP, . - _asm_exc_GP

_asm_exc_PF:
    SAVE_REGS
    call isr_PF_exc
    RESTORE_REGS
    add esp,4
    iretd
    .size   _asm_exc_PF, . - _asm_exc_PF

_asm_schedule:
    SAVE_REGS
    call isr_schedule_int
    mov al,0x20
    out 0x20,al
    RESTORE_REGS
    iretd
    .size   _asm_schedule, . - _asm_schedule

.att_syntax prefix