.section .text

.extern gdt_ptr
.global gdt_flush
gdt_flush:
    lgdt (gdt_ptr) # load the new GDT
    movw $0x10, %ax    # 0x10 points at the new data selector
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ljmp $0x08, $reload_CS
reload_CS:
    ret