#include <kernel/arch.h>
#include <kernel/tty.h>
#include <kernel/i386.h>
#include <stdio.h>

static char cpu_name[512] = "Unknown";
u32 ret_reg[5];
multiboot_info_t *multiboot_info = 0x00;

char *detect() {
  cpu_vendor_name(cpu_name);
  return cpu_name;
}

void init_arch(void) {
  printf("Architecture is x86 (cpu=%s)\n", detect());
  printf("Loading GDT\n");
  init_gdt();
  printf("Loading IDT\n");
  init_idt();
  printf("Enabling pics...\n");
  init_pic();

}

void set_arch_param(u32 ret, u32 ret1, u32 ret2, u32 ret3, u32 ret4) {
  ret_reg[0] = ret;
  ret_reg[1] = ret1;
  ret_reg[2] = ret2;
  ret_reg[3] = ret3;
  ret_reg[4] = ret4;
}

u32 get_arch_arg(u32 n) {
  if (n < 5)
    return ret_reg[n];
  else
    return 0;
}

void kernel_panic(char *error) {
  terminal_setbackground_color(COLOR_RED);
  terminal_clear();
  terminal_setcolor(COLOR_WHITE);
  printf("KERNEL PANIC - %s\nThis is a critical error, please report it to the author!\n", error);
  asm("hlt");
}

void set_multiboot_info(multiboot_info_t *mbt) {
  multiboot_info = mbt;
}

multiboot_info_t *get_multiboot_info(void) {
  return multiboot_info;
}

u64 get_total_ram(void) {
  return get_total_ram_cmos(); // TODO: Use multiboot info
}
