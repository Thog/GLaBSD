.section .multiboot_header
.align 8
multiboot_header_start:
    .long 0xe85250d6                                                                        # magic number
    .long 0                                                                                 # protected mode code
    .long multiboot_header_end - multiboot_header_start                                     # header length
    .long 0x100000000 - (0xe85250d6 + 0 + (multiboot_header_end - multiboot_header_start))  # checksum

    # needed tags go here

    # end tags
    .short 0
    .short 0
    .long 8
multiboot_header_end: