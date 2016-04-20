#include <stdio.h>
#include <kernel/arch.h>
#include <kernel/tty.h>

void kernel_early(multiboot_info_t *mbt, unsigned int magic) {
  terminal_initialize();
  printf("Initializing Kernel core...\n");
  set_multiboot_info(mbt);
  init_arch();
}

void kernel_main(void) {
  printf("Hello, I'm kernel_main!\nWe have %iMB", get_total_ram());
  while (true) {
  }
}
