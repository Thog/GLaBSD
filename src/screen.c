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

static void screen_printkc(char c)
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

static void screen_printk(char *str)
{
    if (!video_base)
        return;

    while (*str)
    {
        screen_printkc(*str++);
    }
}

void printk_int_inner(u32 data, char *base, u32 base_len)
{
    if (data >= base_len)
    {
        printk_int_inner(data / base_len, base, base_len);
        printk_int_inner(data % base_len, base, base_len);
    }
    else
        printkc(base[data]);
}

void printk_int(u32 data, char *base)
{
    u32 base_len = strlen(base);
    if (base_len == 16)
        printk("0x");
    printk_int_inner(data, base, base_len);
}

void printkc(char c)
{
    screen_printkc(c);
}

void printk(char *str)
{
    screen_printk(str);
}