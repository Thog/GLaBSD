#include "kernel.h"

u32 strlen(char *str)
{
    char *base = str;
    while (*str)
        str++;
    return (str - base);
}