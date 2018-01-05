#ifndef FRAMES_H
#define FRAMES_H

#include "kernel.h"

typedef struct
{
    u32 type: 5;
    u32 free : 1;
    u32 reserved : 26;
} frame_t;

void setup_frames(multiboot2_mmap_t *memory_mapping);

#endif