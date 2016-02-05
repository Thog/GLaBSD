#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/types.h>
void kernel_early(void)
{
	terminal_initialize();
}

void kernel_main(void)
{
	printf("Hello, I'm kernel_main!");
}
