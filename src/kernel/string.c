#include "kernel.h"

u32 strlen(char *str)
{
    char *base = str;
    while (*str)
        str++;
    return (str - base);
}

s32 strcmp(char *s1, char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}