ENTRY(_start)

SECTIONS
{
    /* Bootloader is in the first megabyte, respect conventions */
	. = 1M;

    /* put the multiboot header here (also prefer to have another segment for that) */
    .boot :
    {
		*(.multiboot_header)
    }

    /* Text section */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

    /* R only data */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

    /* R/W data */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

    /* BSS */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}