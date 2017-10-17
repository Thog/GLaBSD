section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:


section .text
global _start

extern remap_interrupts
extern mask_interrupts
extern init_gdt
extern main

_start:
    ; Setup the temp stack
    mov esp, stack_top

    ; Remap interrupts
    call remap_interrupts

    ; Mask interrupts
    call mask_interrupts

    ; Setup GDT
    call init_gdt

    ; Kernel main
    call main

    ; Not supposed to return but well, disable interrupts
    cli
.endless:
    jmp .endless
    hlt ; halt the CPU