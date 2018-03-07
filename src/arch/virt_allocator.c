#include "kernel.h"

virt_alloc_t *virt_meta_start;
void *virt_meta_end;
void *next_addr;

static void setup_new_mapping(virt_alloc_t *entry, void *phys_addr, void *virt_addr, size_t len)
{
    entry->phys_addr = (u32)phys_addr >> 12;
    entry->virt_addr = (u32)virt_addr >> 12;
    entry->length = len / PAGE_SIZE;
    entry->used = 1;
    entry->next = NULL;
}

static int new_mapping(void *phys_addr, void *virt_addr, size_t len)
{
    virt_alloc_t *tmp;

    tmp = virt_meta_start;
    while(tmp && tmp->next && tmp->used)
        tmp = tmp->next;
    
    // TODO: extend this
    if ((u32)tmp >= (u32)virt_meta_end || tmp == NULL)
        return 1;
    setup_new_mapping(tmp + 1, phys_addr, virt_addr, len);
    tmp->next = tmp + 1;
    return 0;
}

void virt_init(void)
{
    void *kernel_base = get_kernel_base();
    // We take the another 4MB (20MB in total already in use TODO: rework everything here later)
    virt_meta_start = kernel_base + 0x1000000;
    virt_meta_end = virt_meta_start + 0x400000;
    next_addr = virt_meta_end;
    void * phys_addr = frame_alloc(0x400000);
    mmu_map_page(phys_addr, virt_meta_start, PAGING_RW | PAGING_4MB);
    setup_new_mapping(virt_meta_start, phys_addr, kernel_base, virt_meta_end - kernel_base);
}

static virt_alloc_t *get_mapping(void *addr)
{
    virt_alloc_t *tmp;

    tmp = virt_meta_start;
    while (tmp)
    {
        if (((u32)addr >> 12) == tmp->virt_addr)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

static int is_mapped(void *addr)
{
    virt_alloc_t *tmp;
    u32 virt_addr;

    virt_addr = ((u32)addr >> 12);
    tmp = virt_meta_start;
    while (tmp)
    {
        if (virt_addr >= tmp->virt_addr && virt_addr < (tmp->virt_addr + (tmp->length * PAGE_SIZE)))
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

void *kmmap(void *addr, size_t len)
{
    if (((u32)len % PAGE_SIZE) || is_mapped(addr))
        return NULL;
    void *phys_addr = frame_alloc(len);
    if (phys_addr == NULL)
        return NULL;

    if (!addr)
    {
        addr = next_addr;
        next_addr += len;
    }
    if (new_mapping(phys_addr, addr, len))
    {
        frame_free(phys_addr);
        return NULL;   
    }
    for (size_t i = 0; i < len; i += PAGE_SIZE)
        mmu_map_page(phys_addr + i, addr + i, PAGING_RW);
    return addr;
}

int kmunmap(void *addr, size_t len)
{
    virt_alloc_t *mapping;

    mapping = get_mapping(addr);
    if (!mapping || mapping->length != (len / PAGE_SIZE))
        return -1;

    for (size_t i = 0; i < len; i += PAGE_SIZE)
        mmu_unmap_page(addr + i);
    frame_free((void*)(mapping->phys_addr << 12));
    mapping->phys_addr = 0;
    mapping->virt_addr = 0;
    mapping->length = 0;
    mapping->used = 0;
    return 0;
}