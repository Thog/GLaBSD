section .multiboot_header
align 8
multiboot_header_start:
    dd 0xe85250d6                                                                        ; magic number
    dd 0                                                                                 ; protected mode code
    dd multiboot_header_end - multiboot_header_start                                     ; header length
    dd 0x100000000 - (0xe85250d6 + 0 + (multiboot_header_end - multiboot_header_start))  ; checksum
    ; needed tags go here

    ; end tags
    dw 0
    dw 0
    dd 8
multiboot_header_end: