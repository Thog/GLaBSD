#include "kernel.h"

int main(u32 magic, multiboot2_t *multiboot_data)
{
    init_serial(COM1);
    printk("Serial COM1 started\n");
    printk("Early startup...\n");
    if (magic != 0x36d76289)
    {
        printk("MAGIC MULTBOOT INVALID\n");
        // panic();
        return (0);
    }
    fix_multiboot_tags(multiboot_data);
    screen_init();

    memory_managment_init();
    printk("\nHello from kernel main :)\n");
    return (0);
}