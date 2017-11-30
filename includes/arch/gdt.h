#ifndef GDT_H
#define GDT_H

#include "types.h"

typedef struct
{
    u16 limit; // upper 16 bits of the selector limits
    u32 base; // the base of the gdt entries
} __attribute__((packed)) gdt_ptr_t;

typedef struct
{
    u16 limit_low; // limit 0-15
    u16 base_low; // base 0-15
    u8 base_middle; // base 16-23
    u8  access; // The access byte
    u8  granularity; // limit 16-19 and flags
    u8 base_high; // base 24-31
} __attribute__((packed)) gdt_entry_t;

void set_gdt_entry(s32 index, u32 base, u32 limit, u8 access, u8 flags);
void gdt_flush();


#endif