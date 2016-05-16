#include <kernel/i386.h>
#include <string.h>

regs_t cpu_cpuid(int code) {
  regs_t r;
  asm volatile("cpuid":"=a"(r.eax), "=b"(r.ebx),
  "=c"(r.ecx), "=d"(r.edx):"0"(code));
  return r;
}

u32 cpu_vendor_name(char *name) {
  regs_t r = cpu_cpuid(0x00);

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

  strcpy(name, line1);
  strcat(name, line3);
  strcat(name, line2);
  return 15;
}

u32 *stack_ptr = 0;

void do_syscalls(int num) {
  u32 ret, ret1, ret2, ret3, ret4;
  asm("mov %%ebx, %0": "=m"(ret):);
  asm("mov %%ecx, %0": "=m"(ret1):);
  asm("mov %%edx, %0": "=m"(ret2):);
  asm("mov %%edi, %0": "=m"(ret3):);
  asm("mov %%esi, %0": "=m"(ret4):);
  set_arch_param(ret, ret1, ret2, ret3, ret4);
  asm("cli");
  asm("mov %%ebp, %0": "=m"(stack_ptr):);
  //sys_call (num);
  asm("sti");
}

void remap_isr(u32 offset1, u32 offset2) {
  u8 a1, a2;

  a1 = inb(PIC1_DATA);                        // save masks
  a2 = inb(PIC2_DATA);

  outb(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
  io_wait();
  outb(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);
  io_wait();
  outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
  io_wait();
  outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
  io_wait();
  outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
  io_wait();
  outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
  io_wait();

  outb(PIC1_DATA, ICW4_8086);
  io_wait();
  outb(PIC2_DATA, ICW4_8086);
  io_wait();

  outb(PIC1_DATA, a1);   // restore saved masks.
  outb(PIC2_DATA, a2);
}

void init_pic(void) {
  remap_isr(0x20, 0x70);
  asmv("sti");
}

void init_memory_manager(void) {
  u32 *pd0;       // kernel page directory
  u32 *pt0;       // kernel page table
  u32 page_address;
  int i;

  // create page directory
  pd0 = (u32 *) PD0_ADDR;
  pd0[0] = PT0_ADDR;
  pd0[0] |= 3;
  for (i = 1; i < 1024; i++)
    pd0[i] = 0;

  // create the first page table
  pt0 = (u32 *) PT0_ADDR;
  page_address = 0;
  for (i = 0; i < 1024; i++) {
    pt0[i] = page_address;
    pt0[i] |= 3;
    page_address += 4096;
  }

  asm("mov %0, %%eax    \n \
       mov %%eax, %%cr3 \n \
       mov %%cr0, %%eax \n \
       or %1, %%eax     \n \
       mov %%eax, %%cr0"::"i" (PD0_ADDR), "i" (PAGING_FLAG));
}

void jump_to_task(tss task) {
  asm("cli \n \
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
       iret" : "=m" (task.esp0) : );
}