#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/types.h>
#include <kernel/io.h>
#include <kernel/vga.h>

void terminal_initialize(void);
void terminal_clear(u8 color);
void terminal_putchar(char c);
void terminal_write(const char *data, size_t size);
void terminal_writestring(const char *data);
void terminal_change_cursor_pos(u8 x, u8 y);

#endif
