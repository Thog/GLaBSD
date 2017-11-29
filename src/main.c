#include "kernel.h"

extern multiboot2_t *multiboot_data;

int main(u32 magic, multiboot2_t *data)
{
    init_serial(COM1);
    if (magic != 0x36d76289)
    {
        printk("MAGIC MULTBOOT INVALID\n");
        // panic();
        return (0);
    }
    fix_multiboot_tags(data);
    screen_init();
    memory_managment_init();
    print_multiboot_memory_mapping();
    printk("Hello from kernel main :)\n");
    return (0);
}