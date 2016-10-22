#include <stdio.h>
#include <kernel/arch.h>
#include <kernel/tty.h>
#include <kernel/tasks.h>

void kernel_early(multiboot_info_t *mbt, unsigned int magic) {
  (void)magic;
  terminal_initialize();
  printf("Initializing Kernel core...\n");
  set_multiboot_info(mbt);
  init_arch();
}

void testing_task(void) {
  while (1);
  return;
}

void kernel_main(void) {
  printf("Hello, I'm kernel_main!\n");
  //memcpy((char *) 0x30000, &testing_task, 100); // TODO: ELF loader
  terminal_change_cursor_pos(-1, -1); // Mask cursor
  //terminal_setbackground_color(COLOR_LIGHT_RED);
  //printf("Switching to user task (ring3 mode)\n");
  terminal_setbackground_color(COLOR_BLACK);
  /*asm("     cli \n \
            push $0x33 \n \
            push $0x30000 \n \
            pushfl \n \
            popl %%eax \n \
            orl $0x200, %%eax \n \
            and $0xffffbfff, %%eax \n \
            push %%eax \n \
            push $0x23 \n \
            push $0x0 \n \
            movl $0x20000, %0 \n \
            movw $0x2B, %%ax \n \
            movw %%ax, %%ds \n \
            iret" : "=m" (default_tss.esp0) : );*/
  while (1);
  printf("Critical error, halting system\n");
  asm("hlt");
}
