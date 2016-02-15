#include <kernel/arch.h>
#include <kernel/i386.h>
#include <stdio.h>

static char cpu_name[512] = "x86-unknown";
u32 *stack_ptr;
u32 ret_reg[5];

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
  /*asm("	movw $0x18, %%ax \n \
		movw %%ax, %%ss \n \
		movl %0, %%esp"::"i" (KERN_STACK));
  init_idt ();*/
}

void set_arch_param (u32 ret, u32 ret1, u32 ret2, u32 ret3, u32 ret4)
{
  ret_reg[0] = ret;
  ret_reg[1] = ret1;
  ret_reg[2] = ret2;
  ret_reg[3] = ret3;
  ret_reg[4] = ret4;
}

u32 get_arch_arg (u32 n)
{
  if (n < 5)
    return ret_reg[n];
  else
    return 0;
}
