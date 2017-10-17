.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp

    # Remap interrupts
    call remap_interrupts

    # Mask interrupts
    call mask_interrupts

    # Setup GDT
    call init_gdt

    # Kernel main
    call main
endless:
    jmp endless
    cli # disable interrupts
    hlt # halt the CPU