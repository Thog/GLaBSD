# GRUB headers constants
.set FLAGS,    0x00000003
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

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

    # Initialize %esp registry stack
	movl $stack_top, %esp

	# Initialize the core kernel before running the global constructors.
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
