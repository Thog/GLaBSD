#include "kernel.h"

multiboot2_t *multiboot_data;

multiboot2_tag_t *get_multiboot_tag(u32 id)
{
    multiboot2_tag_t *tags = &multiboot_data->first_tag;
    while (tags->type)
    {
        if (tags->type == id)
            return (tags);
        tags = (multiboot2_tag_t*)((u8*)tags + ((tags->size + 7) & ~7));
    }
    return (NULL);
}

void fix_multiboot_tags(multiboot2_t *data)
{
    multiboot2_elf_section_t *elf;
    u32 va_start;
    u32 i;
    elf32_section_t *elf_section;

    multiboot_data = data;
    elf = (multiboot2_elf_section_t*)get_multiboot_tag(MULTIBOOT_TAG_TYPE_ELF_SECTIONS);
    va_start = (u32)get_kernel_base();
    i = 0;

    if (elf)
    {
        while (i < elf->num)
        {
            elf_section = elf->sections + i;
            
            // Check if the address isn't a virtual one and fix it
            if ((elf_section->addr & va_start) == 0)
            {
                elf_section->addr += va_start;
            }
            i++;
        }
    }
}

static elf32_section_t *get_elf_section(u32 type)
{
    multiboot2_elf_section_t *elf;
    u32 i;

    elf = (multiboot2_elf_section_t*)get_multiboot_tag(MULTIBOOT_TAG_TYPE_ELF_SECTIONS);
    i = 0;
    if (elf)
    {
        while (i < elf->num)
        {
            elf32_section_t *entry = elf->sections + i;
            if (entry->type == type)
                return entry;
            i++;
        }
    }
    return (NULL);
}

static char *get_elf_name(u32 index)
{
    elf32_section_t *strtab;

    strtab = get_elf_section(SHT_STRTAB);
    if (strtab)
        return ((char*)(strtab->addr + index));
    return (NULL);
}

void *get_kernel_symbol(char *name)
{
    elf32_section_t *symtab;
    u32 i;
    elf32_symbol_t *symbol;
    char *symbol_name;

    symtab = get_elf_section(SHT_SYMTAB);
    i = 0;
    if (symtab)
    {
        while (i < symtab-> size && symtab->entry_size != 0)
        {
            symbol = (elf32_symbol_t*)(symtab->addr + i);
            symbol_name = get_elf_name(symbol->name);

            if (symbol_name && !strcmp(name, symbol_name))
            {
                return ((void*)symbol->value);
            }
            i += symtab->entry_size;
        }
    }
    return (NULL);
}