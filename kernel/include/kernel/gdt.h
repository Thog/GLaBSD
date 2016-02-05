#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <kernel/types.h>
struct gdt_desc {
    u16 lim0_15;
    u16 base0_15;
    u8 base16_23;
    u8 acces;
    u8 lim16_19:4;
    u8 other:4;
    u8 base24_31;
} __attribute__ ((packed));

struct gdt_r {
    u16 limit;
    u32 base;
} __attribute__ ((packed));

#endif //_KERNEL_GDT_H
