#ifndef MMU_H
#define MMU_H

#include "kernel.h"

#define PAGING_PRESENT 0x1
#define PAGING_RW 0x2
#define PAGING_USER 0x4
#define PAGING_WRITE_THROUGH 0x8
#define PAGING_DISABLE_CACHE 0x10
#define PAGING_DIRTY 0x40
#define PAGING_4MB 0x80
#define PAGING_GLOBAL 0x100

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

typedef struct
{
    u32 present : 1;
    u32 rw : 1; // 1 = RW, 0 = Read only
    u32 user : 1; // 1 = User, 0 = Supervisor
    u32 write_mode : 1; // 1 = Write-Through 0 = Write-back
    u32 no_cache : 1;
    u32 accessed : 1;
    u32 dirty : 1;
    u32 zero : 1;
    u32 global : 1;
    u32 reserved : 3;
    u32 addr : 20;
} page_table_t;

void mmu_init(void);
void *mmu_map_page(void *phys, void *virt, u32 flags);
void mmu_unmap_page(void *virt);
void mmu_print_page_directory(void);

#endif