#include "kernel.h"

gdt_entry_t gdt_data[5];
gdt_ptr_t gdt_ptr;

void set_gdt_entry(s32 index, u32 base, u32 limit, u8 access, u8 flags)
{
    gdt_data[index].base_low = (base & 0xFFFF);
    gdt_data[index].base_middle = (base >> 16) & 0xFF;
    gdt_data[index].base_high = (base >> 24) & 0xFF;

    gdt_data[index].limit_low = (limit & 0xFFFF);
    gdt_data[index].granularity = ((limit >> 16) & 0x0F) | (flags & 0xF0);
    gdt_data[index].access = access;
}


void init_gdt(void)
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * sizeof(gdt_data)) - 1;
    gdt_ptr.base = (u32)&gdt_data;
    set_gdt_entry(0, 0, 0, 0, 0); // NULL segment
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    set_gdt_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    set_gdt_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
    gdt_flush();
}