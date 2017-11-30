#include "kernel.h"

multiboot2_mmap_t *memory_mapping;
void *va_start = NULL;

void set_kernel_virtual_start(void *addr)
{
    va_start = addr;
}

void *get_kernel_base(void)
{
    return va_start;
}

void memory_managment_init(void)
{
    memory_mapping = (multiboot2_mmap_t*) get_multiboot_tag(MULTIBOOT_TAG_TYPE_MMAP);
    if (!memory_mapping)
    {
        printk("NO MEMORY MAPPING PROVIDED, ABORT\n");
        // panic();
    }
}

void print_multiboot_memory_mapping(void)
{
    multiboot2_mmap_entry_t *entry = (multiboot2_mmap_entry_t*) memory_mapping->entries;
    u64 end_addr;
    printk("====== MEMORY MAPPING ======\n");
    while (entry < (multiboot2_mmap_entry_t*)((u8*)memory_mapping + memory_mapping->size))
    {
        end_addr = entry->base_addr + entry->size;
        printk("ZONE ");
        printk_int(entry->base_addr, BASE_16);
        printk(" - ");
        printk_int(end_addr, BASE_16);
        printk(" : ");
        printk_int(entry->type, BASE_16);
        printk("\n");

        entry = (multiboot2_mmap_entry_t*)((u8*)entry + memory_mapping->entry_size);
    }
}