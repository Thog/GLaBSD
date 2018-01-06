#include "kernel.h"

extern page_directory_t first_page_entry;

static inline void flush_tlb_single(u32 addr)
{
    asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
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

void *mmu_map_page(void *phys, void *virt, u32 is_user, u32 writable)
{
    page_directory_t *entry;

    entry = &first_page_entry + ((u32)virt >> 22);
    entry->addr = ((u32)phys) >> 12;    
    entry->user = is_user == 1;
    entry->size = 1; // TODO: use page tables to map 4KB (we are mapping 4MB for now)
    entry->rw = writable == 1;
    entry->present = 1;
    return virt;
}

void mmu_unmap_page(void *virt)
{
    u32 *entry;

    entry = (u32*)(&first_page_entry +  ((u32)virt >> 22));
    *entry = 0;
    flush_tlb_single((u32)virt);
}
