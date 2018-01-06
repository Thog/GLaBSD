#include "kernel.h"

multiboot2_mmap_t *memory_mapping;
void *va_start = NULL;

extern u32 __va_start__;
extern u32 __va_end__;

void *get_kernel_base(void)
{
    return &__va_start__;
}

void *get_kernel_end(void)
{
    return &__va_end__;
}

void memory_managment_init(void)
{
    memory_mapping = (multiboot2_mmap_t*) get_multiboot_tag(MULTIBOOT_TAG_TYPE_MMAP);
    if (!memory_mapping)
    {
        printk("NO MEMORY MAPPING PROVIDED, ABORT\n");
        // panic();
    }

    setup_frames(memory_mapping);
}

void print_multiboot_memory_mapping(void)
{
    multiboot2_mmap_entry_t *entry;
    u64 end_addr;

    if (!memory_mapping)
        return;
    entry =  (multiboot2_mmap_entry_t*) memory_mapping->entries;
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