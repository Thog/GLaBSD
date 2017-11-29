#include "kernel.h"

void serial_printkc(char c)
{
    write_serial(COM1, c);
}

void serial_printk(char *str)
{
    while (*str)
    {
        serial_printkc(*str);
        str++;
    }
}