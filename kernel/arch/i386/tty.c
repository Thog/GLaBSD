#include <kernel/tty.h>
#include <stdio.h>
size_t x;
size_t y;
u8 terminal_color;
u16 video_port;
uint16_t *terminal_buffer;

void terminal_initialize(void) {
  x = 0;
  y = 0;
  terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
  terminal_buffer = VGA_MEMORY;
  terminal_clear(terminal_color);
  video_port = *((u16 *) 0x0463); // base IO port for video
}

void terminal_setcolor(u8 color) {
  terminal_color = color;
}

void terminal_clear(u8 color) {
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = make_vgaentry(' ', color);
    }
  }
}

void terminal_putentryat(char c, u8 color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c) {
  if (!c) return; // Ignore '\0'
  if (x > VGA_WIDTH || c == '\n') {
    x = 0;
    y++;
  }
  if (y > VGA_HEIGHT - 1)
    y = 0;
  if (c != '\n') {
    terminal_putentryat(c, terminal_color, x, y);
    x++; // Increment X pos
  }
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
  terminal_change_cursor_pos(x, y);
}

void terminal_writestring(const char *data) {
  terminal_write(data, strlen(data));
}

void terminal_change_cursor_pos(u8 x, u8 y) {
  u16 position = (x * 80) + y;
  outb(video_port, 0x0F);
  outb(video_port + 1, (u8) (position & 0xFF));
  outb(video_port, 0x0E);
  outb(video_port + 1, (u8) ((position >> 8) & 0xFF));
}
