#include "kernel.h"

static size_t pos_x = 0;
static size_t pos_y = 0;
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static volatile u16 *video_base = NULL;

void screen_init(u8 *va_start)
{
    video_base = (volatile u16 *)(va_start + 0xB8000);
}

static void screen_print_str(char *str)
{
    if (!video_base)
        return;

    while (*str)
    {
        char c = *str++;
        if (c == '\n')
        {
            pos_x = 0;
            pos_y++;
            continue;
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
}

void printk(char *str)
{
    screen_print_str(str);
}