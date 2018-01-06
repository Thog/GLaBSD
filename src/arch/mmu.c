#include "kernel.h"

extern u32 first_page_entry;
u32 *page_tables;

static inline void flush_tlb_single(u32 addr)
{
    asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void mmu_init()
{
     // We assume that the system has at least 16MB of ram
    page_tables = mmu_map_page((void*)0x400000, get_kernel_base() + 0x400000, PAGING_RW | PAGING_4MB);
    memset(page_tables, 0, 0x400000);

    // used by the frame allocator
    mmu_map_page((void*)0x800000, get_kernel_base() + 0x800000, PAGING_RW | PAGING_4MB);
    mmu_map_page((void*)0xC00000, get_kernel_base() + 0xC00000, PAGING_RW | PAGING_4MB);
}

void *mmu_map_page(void *phys, void *virt, u32 flags)
{
    u32 *page_directory_entry;
    u32 *page_table_entry;
    u32 page_directory_index;

    page_directory_index =  ((u32)virt >> 22);
    page_directory_entry = &first_page_entry + page_directory_index;
    if (flags & PAGING_4MB)
    {
        *page_directory_entry = (u32)phys | (flags & 0xFFF) | PAGING_PRESENT;
    }
    else
    {
        page_table_entry = page_tables + (page_directory_index * 0x400) + ((u32)virt >> 12 & 0x03FF);

        if (*page_directory_entry == 0)
            *page_directory_entry = ((u32)page_table_entry - (u32)get_kernel_base()) | (flags & 0xFFF) | PAGING_PRESENT;
        *page_table_entry = (u32)phys | (flags & 0xFFF) | PAGING_PRESENT;
    }
    flush_tlb_single((u32)phys);
    return virt;
}

void mmu_print_page_directory(void)
{
    u32 *dir;
    u32 i;

    dir = (u32*)&first_page_entry;
    i = 0;
    while (i < 1024)
    {
        printk("Page ");
        printk_int(i << 22, BASE_16);
        printk(": ");
        printk_int(*dir, BASE_2);
        printk("\n");
        dir++;
        i++;
    } 
}

void mmu_unmap_page(void *virt)
{
    u32 *page_directory_entry;
    u32 *page_table_entry;
    u32 page_directory_index;

    page_directory_index =  ((u32)virt >> 22);
    page_directory_entry = &first_page_entry + page_directory_index;
    if (*page_directory_entry & PAGING_4MB)
        *page_directory_entry = 0;
    else
    {
        page_table_entry = page_tables + (page_directory_index * 0x400) + ((u32)virt >> 12 & 0x03FF);
        *page_table_entry = 0;
    }
    flush_tlb_single((u32)virt);
}
