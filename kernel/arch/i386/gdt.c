#include <kernel/i386.h>
#include <string.h>

gdt_desc kgdt[GDTSIZE];
gdt_r kgdtr;

void init_gdt_desc(u32 base, u32 limit, u8 access, u8 other, gdt_desc *desc) {
  desc->lim0_15 = (limit & 0xffff);
  desc->base0_15 = (base & 0xffff);
  desc->base16_23 = (base & 0xff0000) >> 16;
  desc->access = access;
  desc->lim16_19 = (limit & 0xf0000) >> 16;
  desc->other = (other & 0xf);
  desc->base24_31 = (base & 0xff000000) >> 24;
  return;
}

void init_gdt(void) {
  default_tss.debug_flag = 0x00;
  default_tss.io_map = 0x00;
  default_tss.esp0 = 0x20000;
  default_tss.ss0 = 0x18;

  /* initialize gdt segments */
  init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);      // NULL
  init_gdt_desc(0x0, 0xFFFFF, 0x9b, 0x0D, &kgdt[1]);      // Code
  init_gdt_desc(0x0, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);      // Data
  init_gdt_desc(0x0, 0x0, 0x97, 0x0D, &kgdt[3]);      // Stack

  // Userland segment
  init_gdt_desc(0x30000, 0x1, 0xFF, 0x0D, &kgdt[4]);    // user code
  init_gdt_desc(0x30000, 0x1, 0xF3, 0x0D, &kgdt[5]);    // user data
  init_gdt_desc(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);        // user stack
  init_gdt_desc((u32) &default_tss, 0x67, 0xE9, 0x00, &kgdt[7]);    // TSS descriptor

  /* initialize the gdtr structure */
  kgdtr.limit = GDTSIZE * 8;
  kgdtr.base = GDTBASE;

  /* copy the gdtr to its memory area */
  memcpy((void *) kgdtr.base, kgdt, kgdtr.limit);

  /* load the gdtr registry */
  asm("lgdtl (kgdtr)");

  /* initiliaze the segments */
  asm("     movw $0x10, %ax	\n \
            movw %ax, %ds	\n \
            movw %ax, %es	\n \
            movw %ax, %fs	\n \
            movw %ax, %gs	\n \
            ljmp $0x08, $next	\n \
            next:		\n");

}