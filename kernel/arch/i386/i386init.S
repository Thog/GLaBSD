.intel_syntax noprefix

.global _asm_syscalls
.global _asm_exc_GP
.global _asm_exc_PF
.global _asm_schedule

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

.macro INTERRUPT id
    .global _asm_int_\id
    _asm_int_\id:
        SAVE_REGS
        push \id
        call isr_default_int
        pop eax
        mov al,0x20
        out 0x20, al
        RESTORE_REGS
        iretd
    .size   _asm_int_\id, . - _asm_int_\id
.endm

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

INTERRUPT 0
INTERRUPT 1
INTERRUPT 2
INTERRUPT 3
INTERRUPT 4
INTERRUPT 5
INTERRUPT 6
INTERRUPT 7
INTERRUPT 8
INTERRUPT 9

.att_syntax prefix