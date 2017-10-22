#include "kernel.h"

int main(u32 multiboot_magic, multiboot2_t *mbt)
{
    if (multiboot_magic != 0x36d76289)
    {
        printk("MAGIC MULTBOOT INVALID\n");
        return (0);
    }
    printk("Hello from kernel main :)\n");
    return (0);
}