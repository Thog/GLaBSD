section .text
align 4
global _start_virtual
extern remap_interrupts
extern mask_interrupts
extern init_gdt
extern main
extern first_page_entry
extern __va_start__
extern set_kernel_virtual_start

_start_virtual:
    ; unmap the first identity page
    mov dword [first_page_entry], 0
    invlpg [0]

    push eax ; backup multiboot magic

    ; remap interrupts
    call remap_interrupts

    ; mask interrupts
    call mask_interrupts

    ; setup GDT (FIXME: CAN'T DO THAT AFTER PAGING IT SHOULD BE IN THE STAGE1)
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