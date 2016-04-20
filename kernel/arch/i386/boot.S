# Declare constants used for creating a multiboot header.
.set FLAGS,    0x00000003       # this is the Multiboot 'flag' field (ALIGN | MEMINFO)
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Provide our own stack to esp registry (GRUB boot infos)
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global kernel_init
.type kernel_init, @function
kernel_init:
	# Initialize the core kernel before running the global constructors.
	movl $stack_top, %esp
	call kernel_early

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
.Lhang:
	hlt
	jmp .Lhang
.size kernel_init, . - kernel_init
