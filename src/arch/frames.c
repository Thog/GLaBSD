#include "kernel.h"

frame_t *frames_start;
frame_t *frames_end;


void setup_frames(multiboot2_mmap_t *memory_mapping)
{
    multiboot2_mmap_entry_t *entry;
    u64 end_addr;

    entry = (multiboot2_mmap_entry_t*) memory_mapping->entries;

    frames_start = get_kernel_base() + 0x800000;
    frames_end = frames_start;

    // Clean up 8MB
    bzero(frames_start, 0x800000);
    while (entry < (multiboot2_mmap_entry_t*)((u8*)memory_mapping + memory_mapping->size))
    {
        end_addr = entry->base_addr + entry->size;
        // memory zone availaible?
        if (entry->type == 1)
        {
            for (u32 addr = entry->base_addr; addr < end_addr; addr += PAGE_SIZE)
            {
                frames_end->addr = addr >> 12;
                frames_end->free = addr >= 0x1000000;
                frames_end++;
            }
        }
        entry = (multiboot2_mmap_entry_t*)((u8*)entry + memory_mapping->entry_size);
    }
}

void print_frame(frame_t *frame)
{
    printk("Frame ");
    printk_int(frame->addr << 12, BASE_16);
    printk(" : free ");
    printk_int(frame->free, BASE_10);
    printk(", contiguous ");
    printk_int(frame->contiguous, BASE_10);
    printk(", meta ");
    printk_int(frame->meta, BASE_16);
    printk("\n");
}

void print_frames(void)
{
    frame_t *frame;

    frame = frames_start;
    while (frame != frames_end)
    {
        if (frame->free == 0)
        {
            print_frame(frame);
        }
        frame++;
    }
}

// Simple frame_alloc, O(N)
// TODO: improve this
void *frame_alloc(size_t size)
{
    frame_t *frame;
    frame_t *tmp_frame;
    u32 page_count;
    u32 contiguous_page;

    size = size == 0 ? PAGE_SIZE : size;
    tmp_frame = frames_start;
    page_count = size / PAGE_SIZE;
    contiguous_page = 0;
    frame = NULL;
    if (page_count == 0)
        return NULL;
    while (tmp_frame != frames_end && contiguous_page != page_count)
    {
        if (tmp_frame->free)
        {
            if (contiguous_page == 0)
            {
                frame = tmp_frame;
                contiguous_page++;
            }
            else if (frame && (frame->addr + contiguous_page) == tmp_frame->addr)
                contiguous_page++;
            else
            {
                frame = NULL;
                contiguous_page = 0;
            }
        }
        tmp_frame++;
    }

    // Enougth pages?
    if (contiguous_page == page_count)
    {
        frame->free = 0;
        frame->meta = page_count;
        frame->contiguous = 0;

        tmp_frame = frame + 1;
        contiguous_page = contiguous_page - 1;
        
        while (contiguous_page--)
        {
            tmp_frame->free = 0;
            tmp_frame->meta = (u32)frame;
            tmp_frame->contiguous = 1;
            tmp_frame++;
        }
        //print_frame(frame);
        return ((void*)(frame->addr << 12));
    }
    return NULL;
}

void frame_free(void *phys)
{
    frame_t *frame;
    u32 addr;
    u32 page_count;

     addr = (u32)phys >> 12;
     frame = frames_start;
     while (frame != frames_end)
     {
        if (frame->addr == addr)
        {
            if (frame->contiguous == 1 && frame->meta >= (u32)frames_start && frame->meta < (u32)frames_end)
            {
                frame = (frame_t*)frame->meta;
            }
            page_count = frame->meta;
            frame->meta = 0;
            frame->contiguous = 0;
            frame->free = 1;
            while (page_count != 0)
            {
                frame->contiguous = 0;
                frame->meta = 0;
                frame->free = 1;
                page_count--;
                frame++;
            }
            break;
        }
        frame++;
    }
}