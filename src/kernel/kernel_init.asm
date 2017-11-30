section .text
align 4
global _start_virtual
extern remap_interrupts
extern mask_interrupts
extern init_gdt
extern main
extern page_directory
extern __va_start__
extern set_kernel_virtual_start

_start_virtual:
    ; unmap the first identity page
    mov dword [page_directory], 0
    invlpg [0]

    push eax ; backup multiboot magic

    ; setup the screen va_start
    lea eax, [__va_start__]
    push eax
    call set_kernel_virtual_start
    add esp, 4

    ; remap interrupts
    call remap_interrupts

    ; mask interrupts
    call mask_interrupts

    ; setup GDT
    call init_gdt

    pop eax ; restore multiboot magic
    add ebx, __va_start__ ; the multiboot ptr must be in the va range
    push ebx ; multiboot ptr
    push eax ; multiboot magic
    ; kernel main
    call main

    cli
.endless:
    jmp .endless
    hlt ; halt the CPU