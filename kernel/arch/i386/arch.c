#include <kernel/arch.h>
#include <kernel/i386.h>
#include <stdio.h>

static char cpu_name[512] = "x86-unknown";

char *detect ()
{
  cpu_vendor_name (cpu_name);
  return cpu_name;
}

void init_arch (void)
{
  printf ("Architecture is x86 (cpu=%s)\n", detect ());
  printf ("Loading GDT \n");
  init_gdt ();
  asm("	movw $0x18, %%ax \n \
			movw %%ax, %%ss \n \
			movl %0, %%esp"::"i" (KERN_STACK));
}