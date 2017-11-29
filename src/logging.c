#include "kernel.h"

void printk_int_inner(u64 data, char *base, u32 base_len)
{
    if (data >= base_len)
    {
        printk_int_inner(data / base_len, base, base_len);
        printk_int_inner(data % base_len, base, base_len);
    }
    else
        printkc(base[data]);
}

void printk_int(u64 data, char *base)
{
    u32 base_len = strlen(base);
    if (base_len == 16)
        printk("0x");
    printk_int_inner(data, base, base_len);
}