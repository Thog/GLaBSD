#include "kernel.h"

static size_t pos_x = 0;
static size_t pos_y = 0;
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static volatile u16 *video_base = NULL;

void screen_init(void)
{
    video_base = (volatile u16 *)(((u8*)get_kernel_base()) + 0xB8000);
}

void screen_printkc(char c)
{
    if (c == '\n')
    {
        pos_x = 0;
        pos_y++;
        return;
    }
    *(video_base + pos_y * VGA_WIDTH + pos_x++) = c | 0xF00;
    if (pos_x == VGA_WIDTH)
    {
        pos_x = 0;
        pos_y++;
    }
    if (pos_y == VGA_HEIGHT)
    {
        pos_x = 0;
        pos_y = 0;
    }
}

void screen_printk(char *str)
{
    if (!video_base)
        return;

    while (*str)
    {
        screen_printkc(*str++);
    }
}
