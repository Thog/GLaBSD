#ifndef _KERNEL_I386_H
#define _KERNEL_I386_H
#define GDTSIZE    0xFF // Max descriptors in the table
#define GDTBASE 0x00000800 // address of GDT
#define IDTBASE 0x00000000 // address of IDT
#define IDTSIZE 0xFF // Max descriptors in the table
#define IDTBYTESIZE 0x800
#define KERN_STACK 0x0009FFF0


#define INTGATE  0x8E00        /* used to manage interrupts calls */
#define TRAPGATE 0xEF00        /*  used to manage system calls */

#include <kernel/types.h>
#include <kernel/io.h>

typedef struct {
  u16 previous_task, __previous_task_unused;
  u32 esp0;
  u16 ss0, __ss0_unused;
  u32 esp1;
  u16 ss1, __ss1_unused;
  u32 esp2;
  u16 ss2, __ss2_unused;
  u32 cr3;
  u32 eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
  u16 es, __es_unused;
  u16 cs, __cs_unused;
  u16 ss, __ss_unused;
  u16 ds, __ds_unused;
  u16 fs, __fs_unused;
  u16 gs, __gs_unused;
  u16 ldt_selector, __ldt_sel_unused;
  u16 debug_flag, io_map;
} __attribute__ ((packed)) tss;

typedef struct {
  u16 lim0_15;
  u16 base0_15;
  u8 base16_23;
  u8 access;
  u8 lim16_19 : 4;
  u8 other : 4;
  u8 base24_31;
} __attribute__ ((packed)) gdt_desc;

typedef struct {
  u16 offset0_15;
  u16 select;
  u16 type;
  u16 offset16_31;
} __attribute__ ((packed)) idt_desc;

typedef struct {
  u16 limit;
  u32 base;
} __attribute__ ((packed)) gdt_r;

typedef struct {
  u16 limit;
  u32 base;
} __attribute__ ((packed)) idt_r;

typedef struct {
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
  u32 ds, es, fs, gs;
  u32 which_int, err_code;
  u32 eip, cs, eflags, user_esp, user_ss;
} __attribute__((packed)) regs_t;

// cpu detection
regs_t cpu_cpuid(int code);
u32 cpu_vendor_name(char *str);

// GDT init
void init_gdt_desc(u32 base, u32 limit, u8 access, u8 other, gdt_desc *desc);
void init_gdt(void);

// IDT init
void init_idt_desc(u16 select, u32 offset, u16 type, idt_desc *desc);
void init_idt(void);

void init_pic(void);

#endif //_KERNEL_I386_H
