ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
	/* Bootloader is in the first megabyte, respect conventions */
	. = 1M;

	__boot_start__ = .;
    /* put the kernel starter here */
	.boot ALIGN(4K) :
	{
		*(.multiboot_header)
		*(.boot)
	}
	__boot_end__ = .;

	/* Start of the kernel VA */
	PROVIDE(__va_start__ = 0xF0000000);
	. += __va_start__;

	/* Text section */
	.text ALIGN(4K) : AT(ADDR(.text) - __va_start__)
	{
		*(.text)
	}

	/* R only data */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - __va_start__)
	{
		*(.rodata*)
	}

	/* R/W data */
	.data ALIGN(0x100) : AT(ADDR(.data) - __va_start__)
	{
		*(.data)
	}

	.eh_frame ALIGN(4K) : AT(ADDR(.eh_frame) - __va_start__)
	{
		*(.eh_frame)
	}

	/* BSS */
	.bss ALIGN(4K) : AT(ADDR(.bss) - __va_start__)
	{
		*(COMMON)
		*(.bss)
	}

	/DISCARD/ :
	{
		*(.comment)
	}
}
