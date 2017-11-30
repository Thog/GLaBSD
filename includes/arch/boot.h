#ifndef BOOT_H
#define BOOT_H

#include "types.h"

#define MULTIBOOT_TAG_TYPE_END 0
#define MULTIBOOT_TAG_TYPE_CMD_LINE 1
#define MULTIBOOT_TAG_TYPE_BOOTLOADER_NAME  2
#define MULTIBOOT_TAG_TYPE_MODULE 3
#define MULTIBOOT_TAG_TYPE_BASIC_MEMINFO 4
#define MULTIBOOT_TAG_TYPE_BOOTDEV 5
#define MULTIBOOT_TAG_TYPE_MMAP 6
#define MULTIBOOT_TAG_TYPE_VBE 7
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8
#define MULTIBOOT_TAG_TYPE_ELF_SECTIONS 9
#define MULTIBOOT_TAG_TYPE_APM 10

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3

typedef struct multiboot2_tag_s
{
    u32 type;
    u32 size;
} multiboot2_tag_t;

typedef struct multiboot2_s
{
    u32 total_size;
    u32 reserved;
    multiboot2_tag_t first_tag;
} multiboot2_t;

typedef struct multiboot2_mmap_entry_s
{
    u64 base_addr;
    u64 size;
    u32 type;
    u32 reserved;
} __attribute__((packed)) multiboot2_mmap_entry_t;

typedef struct multiboot2_mmap_s
{
    u32 type;
    u32 size;
    u32 entry_size;
    u32 entry_version;
    multiboot2_mmap_entry_t entries[0];
} multiboot2_mmap_t;

typedef struct elf32_section_s
{
    u32 name;
    u32 type;
    u32 flags;
    u32 addr;
    u32 offset;
    u32 size;
    u32 link;
    u32 info;
    u32 align;
    u32 entry_size;

} elf32_section_t;

typedef struct elf32_symbol_s
{
    u32 name;
    u32 value;
    u32 size;
    u8 info;
    u8 other;
    u16 shndx;
} elf32_symbol_t;

typedef struct multiboot2_elf_section_s
{
    u32 type;
    u32 size;
    u32 num;
    u32 entry_size;
    u32 shndx;
    elf32_section_t sections[0];
} multiboot2_elf_section_t;


static inline void outb(u16 port, u8 val)
{
    asm volatile("outb %0, %1" :: "a"(val), "Nd"(port));
}

static inline void outw(u16 port, u16 val)
{
    asm volatile("outw %0, %1" :: "a"(val), "Nd"(port));
}

static inline void outl(u16 port, u32 val)
{
    asm volatile("outl %0, %1" :: "a"(val), "Nd"(port));
}

static inline void io_wait(void)
{
    // Linux kernel is doing this like that :/
    outb(0x80, 0);
}

static inline void outb_p(u16 port, u8 val)
{
    outb(port, val);
    io_wait();
}

static inline u8 inb(u16 port)
{
    u8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline u16 inw(u16 port)
{
    u16 ret;
    asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline u32 inl(u16 port)
{
    u32 ret;
    asm volatile("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

multiboot2_tag_t *get_multiboot_tag(u32 id);
void print_multiboot_memory_mapping(void);
void fix_multiboot_tags(multiboot2_t *data);
void *get_kernel_symbol(char *name);

#endif