#ifndef FRAMES_H
#define FRAMES_H

#include "kernel.h"

typedef struct
{
    u32 addr : 20;
    u32 free : 1;
    u32 contiguous : 1;
    u32 meta; // if contiguous = 1, address of the primary frame, otherwise the actual page_count
    u32 reserved : 10;
} frame_t;

void setup_frames(multiboot2_mmap_t *memory_mapping);
void print_frames(void);
void print_frame(frame_t *frame);
void *frame_alloc(size_t size);
void frame_free(void *phys);

#endif