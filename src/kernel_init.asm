section .text
align 4
global _start_virtual
extern remap_interrupts
extern mask_interrupts
extern init_gdt
extern main
extern page_directory
extern __va_start__
extern screen_init

_start_virtual:
    ; unmap the first identity page
    mov dword [page_directory], 0
    invlpg [0]

    push eax ; backup multiboot magic

    ; setup the screen va_start
    lea eax, [__va_start__]
    push eax
    call screen_init
    add esp, 4

    ; remap interrupts
    call remap_interrupts

    ; mask interrupts
    call mask_interrupts

    ; setup GDT
    call init_gdt

    pop eax ; restore multiboot magic
    push ebx ; multiboot ptr
    push eax ; multiboot magic
    ; kernel main
    call main

    cli
.endless:
    jmp .endless
    hlt ; halt the CPU