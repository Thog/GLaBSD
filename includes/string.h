#ifndef STRING_H
#define STRING_H

#include "types.h"

u32 strlen(char *str);
s32 strcmp(char *s1, char *s2);
void *memset(void *s, int c, size_t n);
void bzero(void *s, size_t n);

#endif