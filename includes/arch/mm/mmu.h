#ifndef MMU_H
#define MMU_H

#include "kernel.h"

typedef struct
{
    u32 present : 1;
    u32 rw : 1; // 1 = RW, 0 = Read only
    u32 user : 1; // 1 = User, 0 = Supervisor
    u32 write_mode : 1; // 1 = Write-Through 0 = Write-back
    u32 no_cache : 1;
    u32 accessed : 1;
    u32 zero : 1;
    u32 size : 1; // 1 = 4MB page, 0 = 4KB manage by page table
    u32 global : 1;
    u32 reserved : 3;
    u32 addr : 20; // addr = real_addr >> 12
} page_directory_t;

void *mmu_map_page(void *phys, void *virt, u32 is_user, u32 writable);
void mmu_print_page_directory(void);

#endif