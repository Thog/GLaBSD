#include <stdio.h>
#include <kernel/arch.h>
#include <kernel/tty.h>

void kernel_early(void) {
  terminal_initialize();
  printf("Initializing Kernel core...\n");
  init_arch();
}

void kernel_main(void) {
  printf("Hello, I'm kernel_main!\n");
  asm("INT $0xFE");
  while (true) {
  }
}
