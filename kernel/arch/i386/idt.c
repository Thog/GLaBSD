#include <kernel/i386.h>

/* IDT table */
idt_desc kidt[IDTSIZE];

/* IDTR registry */
idt_r kidtr;


extern void _asm_int_0();

extern void _asm_int_1();

extern void _asm_int_2();

extern void _asm_int_3();

extern void _asm_int_4();

extern void _asm_int_5();

extern void _asm_int_6();

extern void _asm_int_7();

extern void _asm_int_8();

extern void _asm_int_9();

extern void _asm_int_33(); // Keyboard

extern void _asm_syscalls();

extern void _asm_exc_GP(void);

extern void _asm_exc_PF(void);

extern void _asm_schedule();

void isr_default_int(int id) {
  switch (id) {
    case 0:
      kernel_panic("Division by 0!");
      break;
    case 6:
      kernel_panic("Invalid Opcode");
      break;
    case 33:
      isr_kbd_int();
      break;
    default:
      printf("Unknown Interrupt: %i\n", id);
      return;
  }
  outb(0x20, 0x20);
  outb(0xA0, 0x20);
}

void isr_PF_exc(void) {
  u32 fault_addr, code;
  u32 eip;
  u32 stack;
  asm(" 	movl 60(%%ebp), %%eax	\n \
    		mov %%eax, %0		\n \
		mov %%cr2, %%eax	\n \
		mov %%eax, %1		\n \
 		movl 56(%%ebp), %%eax	\n \
    		mov %%eax, %2"
  : "=m"(eip), "=m"(fault_addr), "=m"(code));
  asm("mov %%ebp, %0": "=m"(stack):);

}

void isr_schedule_int() {
  static int tic = 0;
  static int sec = 0;
  tic++;
  if (tic % 100 == 0) {
    sec++;
    tic = 0;
  }
  outb(0x20, 0x20);
  outb(0xA0, 0x20);
}

void isr_GP_exc(void) {
  kernel_panic("General Protection fault!");
}

void init_idt_desc(u16 select, u32 offset, u16 type, idt_desc *desc) {
  desc->offset0_15 = (offset & 0xffff);
  desc->select = select;
  desc->type = type;
  desc->offset16_31 = (offset & 0xffff0000) >> 16;
}

void init_idt(void) {

  init_idt_desc(0x08, (u32) _asm_int_0, INTGATE, &kidt[0]);
  init_idt_desc(0x08, (u32) _asm_int_1, INTGATE, &kidt[1]);
  init_idt_desc(0x08, (u32) _asm_int_2, INTGATE, &kidt[2]);
  init_idt_desc(0x08, (u32) _asm_int_3, INTGATE, &kidt[3]);
  init_idt_desc(0x08, (u32) _asm_int_4, INTGATE, &kidt[4]);
  init_idt_desc(0x08, (u32) _asm_int_5, INTGATE, &kidt[5]);
  init_idt_desc(0x08, (u32) _asm_int_6, INTGATE, &kidt[6]);
  init_idt_desc(0x08, (u32) _asm_int_7, INTGATE, &kidt[7]);
  init_idt_desc(0x08, (u32) _asm_int_8, INTGATE, &kidt[8]);
  init_idt_desc(0x08, (u32) _asm_int_9, INTGATE, &kidt[9]);
  /* Vectors  0 -> 31 are for exceptions */
  init_idt_desc(0x08, (u32) _asm_exc_GP, INTGATE, &kidt[13]);     // General Protection Fault
  init_idt_desc(0x08, (u32) _asm_exc_PF, INTGATE, &kidt[14]);     // Page Fault

  init_idt_desc(0x08, (u32) _asm_schedule, INTGATE, &kidt[32]); // Programmable Interrupt Timer Interrupt
  init_idt_desc(0x08, (u32) _asm_int_33, INTGATE, &kidt[33]); // Keyboard Interrupt

  init_idt_desc(0x08, (u32) _asm_syscalls, TRAPGATE, &kidt[48]);
  init_idt_desc(0x08, (u32) _asm_syscalls, TRAPGATE, &kidt[128]); //48

  kidtr.limit = IDTBYTESIZE;
  kidtr.base = IDTBASE;
  /* Copy the IDT to the memory */
  memcpy((char *) kidtr.base, (char *) kidt, IDTBYTESIZE);
  /* Load the IDTR registry */
  asm("lidtl (kidtr)");
}