section .text

extern gdt_ptr

global gdt_flush

gdt_flush:
    lgdt [gdt_ptr] ; load the new GDT

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:reload_CS
reload_CS:
    ret