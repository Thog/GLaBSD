#include "kernel.h"

frame_t *frames;

void setup_frames(multiboot2_mmap_t *memory_mapping)
{
    multiboot2_mmap_entry_t *entry = (multiboot2_mmap_entry_t*) memory_mapping->entries;
    u64 end_addr;
    frame_t *current_frame;

     // We assume that the system has at least 8MB of ram
    frames = mmu_map_page((void*)0x400000, get_kernel_base() + 0x400000, 0, 1);
    // Clean the 4MB
    memset(frames, 0, 0x400000);
    while (entry < (multiboot2_mmap_entry_t*)((u8*)memory_mapping + memory_mapping->size))
    {
        end_addr = entry->base_addr + entry->size;
        // memory zone availaible?
        if (entry->type == 1)
        {
            for (u32 addr = entry->base_addr; addr < end_addr; addr += PAGE_SIZE)
            {
                current_frame = frames + (addr >> 22);
                current_frame->type = entry->type;
                current_frame->free = 1;
            }
        }
        entry = (multiboot2_mmap_entry_t*)((u8*)entry + memory_mapping->entry_size);
    }
}