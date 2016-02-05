#include <kernel/i386.h>
#include <string.h>

regs_t cpu_cpuid (int code)
{
  regs_t r;
  asm volatile("cpuid":"=a"(r.eax), "=b"(r.ebx),
  "=c"(r.ecx), "=d"(r.edx):"0"(code));
  return r;
}

u32 cpu_vendor_name (char *name)
{
  regs_t r = cpu_cpuid (0x00);

  char line1[5];
  line1[0] = ((char *) &r.ebx)[0];
  line1[1] = ((char *) &r.ebx)[1];
  line1[2] = ((char *) &r.ebx)[2];
  line1[3] = ((char *) &r.ebx)[3];
  line1[4] = '\0';

  char line2[5];
  line2[0] = ((char *) &r.ecx)[0];
  line2[1] = ((char *) &r.ecx)[1];
  line2[2] = ((char *) &r.ecx)[2];
  line2[3] = ((char *) &r.ecx)[3];
  line2[4] = '\0';

  char line3[5];
  line3[0] = ((char *) &r.edx)[0];
  line3[1] = ((char *) &r.edx)[1];
  line3[2] = ((char *) &r.edx)[2];
  line3[3] = ((char *) &r.edx)[3];
  line3[4] = '\0';

  strcpy (name, line1);
  strcat (name, line3);
  strcat (name, line2);
  return 15;
}


// GDT PART

/* GDT */
gdt_desc kgdt[GDTSIZE];

tss default_tss;

/* GDTR */
gdt_r kgdtr;

u32 *stack_ptr = 0;

void init_gdt_desc (u32 base, u32 limite, u8 acces, u8 other, gdt_desc *desc)
{
  desc->lim0_15 = (limite & 0xffff);
  desc->base0_15 = (base & 0xffff);
  desc->base16_23 = (base & 0xff0000) >> 16;
  desc->acces = acces;
  desc->lim16_19 = (limite & 0xf0000) >> 16;
  desc->other = (other & 0xf);
  desc->base24_31 = (base & 0xff000000) >> 24;
  return;
}

void init_gdt (void)
{
  default_tss.debug_flag = 0x00;
  default_tss.io_map = 0x00;
  default_tss.esp0 = 0x1FFF0;
  default_tss.ss0 = 0x18;

  /* initialize gdt segments */
  init_gdt_desc (0x0, 0x0, 0x0, 0x0, &kgdt[0]);
  init_gdt_desc (0x0, 0xFFFFF, 0x9B, 0x0D, &kgdt[1]);    /* code */
  init_gdt_desc (0x0, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);    /* data */
  init_gdt_desc (0x0, 0x0, 0x97, 0x0D, &kgdt[3]);        /* stack */

  init_gdt_desc (0x0, 0xFFFFF, 0xFF, 0x0D, &kgdt[4]);    /* ucode */
  init_gdt_desc (0x0, 0xFFFFF, 0xF3, 0x0D, &kgdt[5]);    /* udata */
  init_gdt_desc (0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);        /* ustack */

  init_gdt_desc ((u32) &default_tss, 0x67, 0xE9, 0x00, &kgdt[7]);    /* TSS descriptor */

  /* initialize the gdtr structure */
  kgdtr.limit = GDTSIZE * 8;
  kgdtr.base = GDTBASE;

  /* copy the gdtr to its memory area */
  memcpy ((char *) kgdtr.base, (char *) kgdt, kgdtr.limit);

  /* load the gdtr registry */
  asm("lgdtl (kgdtr)");

  /* initiliaze the segments */
  asm("   movw $0x10, %ax	\n \
            movw %ax, %ds	\n \
            movw %ax, %es	\n \
            movw %ax, %fs	\n \
            movw %ax, %gs	\n \
            ljmp $0x08, $next	\n \
            next:		\n");

}