#ifndef VIRT_ALLOCATOR_H
#define VIRT_ALLOCATOR_H

#include "kernel.h"

#define EMAP_MAPPED -1
#define EMAP_NOMEM -2
#define EMAP_UNKNOWN -3

typedef struct virt_alloc_s
{
    u32 virt_addr : 20;
    u32 phys_addr : 20;
    size_t length : 23; // max is 63GB
    u32 used : 1;
    struct virt_alloc_s *next;
} virt_alloc_t;

void virt_init(void);
void *kmmap(void *addr, size_t len);
int kmunmap(void *addr, size_t len);

#endif