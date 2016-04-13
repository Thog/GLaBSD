#include <kernel/i386.h>
#include <kernel/arch.h>
#include <string.h>
#include <stdio.h>

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

extern void _asm_syscalls();

extern void _asm_exc_GP(void);

extern void _asm_exc_PF(void);

extern void _asm_schedule();

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


// GDT PART

/* GDT */
gdt_desc kgdt[GDTSIZE];

tss default_tss;

/* GDTR */
gdt_r kgdtr;

/* IDT table */
idt_desc kidt[IDTSIZE];

/* IDTR registry */
idt_r kidtr;

u32 *stack_ptr = 0;

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

void init_idt_desc(u16 select, u32 offset, u16 type, idt_desc *desc) {
    desc->offset0_15 = (offset & 0xffff);
    desc->select = select;
    desc->type = type;
    desc->offset16_31 = (offset & 0xffff0000) >> 16;
}

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

void init_gdt(void) {
    default_tss.debug_flag = 0x00;
    default_tss.io_map = 0x00;
    default_tss.esp0 = 0x1FFF0;
    default_tss.ss0 = 0x18;

    /* initialize gdt segments */
    init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);      // NULL
    init_gdt_desc(0x0, 0xFFFFF, 0x9b, 0x0D, &kgdt[1]);      // Code
    init_gdt_desc(0x0, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);      // Data
    init_gdt_desc(0x0, 0x0, 0x97, 0x0D, &kgdt[3]);      // Stack

    /* initialize the gdtr structure */
    kgdtr.limit = GDTSIZE * 8;
    kgdtr.base = GDTBASE;

    /* copy the gdtr to its memory area */
    memcpy((char *) kgdtr.base, (char *) kgdt, kgdtr.limit);

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

void init_idt(void) {


    memset(0, 0, sizeof(idt_desc) * IDTSIZE);
    printf("Bzero kidt...\n");
    /* Init irq */
    int i;
    for (i = 0; i < IDTSIZE; i++)
        init_idt_desc(0x08, (u32) _asm_schedule, INTGATE, &kidt[i]); //

    init_idt_desc(0x08, (u32) _asm_int_0, INTGATE, &kidt[0]);
    /* Vectors  0 -> 31 are for exceptions */
    init_idt_desc(0x08, (u32) _asm_exc_GP, INTGATE, &kidt[13]);     /* #GP */
    init_idt_desc(0x08, (u32) _asm_exc_PF, INTGATE, &kidt[14]);     /* #PF */

    init_idt_desc(0x08, (u32) _asm_schedule, INTGATE, &kidt[32]);
    init_idt_desc(0x08, (u32) _asm_int_1, INTGATE, &kidt[33]);

    init_idt_desc(0x08, (u32) _asm_syscalls, TRAPGATE, &kidt[48]);
    init_idt_desc(0x08, (u32) _asm_syscalls, TRAPGATE, &kidt[128]); //48

    kidtr.limit = IDTBYTESIZE;
    kidtr.base = IDTBASE;
    /* Copy the IDT to the memory */
    memcpy((char *) kidtr.base, (char *) kidt, IDTBYTESIZE);
    /* Load the IDTR registry */
    asm("lidtl (kidtr)");
}

void outb(u16 ad, u8 v) {
    asmv("outb %0, %1" : : "a"(v), "Nd"(ad));
}

void isr_default_int(int id) {
    switch (id) {
        case 0:
            kernel_panic("Division by 0!");
            break;
        case 1:
            printf("Keyboard ISR!\n");
            //isr_kbd_int ();
            break;
        default:
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

void init_pic(void) {
    /* Initialization of ICW1 */
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    /* Initialization of ICW2 */
    outb(0x21, 0x20);    /* start vector = 32 */
    outb(0xA1, 0x70);    /* start vector = 96 */

    /* Initialization of ICW3 */
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    /* Initialization of ICW4 */
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    /* mask interrupts */
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}