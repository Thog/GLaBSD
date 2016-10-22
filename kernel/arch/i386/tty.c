#include <kernel/tty.h>
#include <stdio.h>
size_t x;
size_t y;
u8 terminal_color;
u8 background_color;
u16 video_port;
uint16_t *terminal_buffer;

void terminal_initialize(void) {
  x = 0;
  y = 0;
  terminal_color = COLOR_LIGHT_GREY;
  background_color = COLOR_BLACK;
  terminal_buffer = VGA_MEMORY;
  terminal_clear();
  video_port = *((u16 *) 0x0463); // base IO port for video
}

void terminal_setcolor(u8 color) {
  terminal_color = color;
}

void terminal_setbackground_color(u8 color) {
  background_color = color;
}

void terminal_clear(void) {
  for (size_t i = 0; i < VGA_HEIGHT; i++) {
    for (size_t j = 0; j < VGA_WIDTH; j++) {
      const size_t index = i * VGA_WIDTH + j;
      terminal_buffer[index] = make_vgaentry(' ', terminal_color, background_color);
    }
  }
  x = 0;
  y = 0;
}

void terminal_putentryat(char c, u8 color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = make_vgaentry(c, color, background_color);
}

void terminal_putchar(char c) {
  if (!c) return; // Ignore '\0'
  if (c == '\b') {
    --x;
    if (!x && y) {
      x = VGA_WIDTH;
      --y;
    }
    terminal_putentryat(' ', terminal_color, x, y);
  }
  else {
    if (x > VGA_WIDTH || c == '\n') {
      x = 0;
      y++;
    }
    if (y > VGA_HEIGHT - 1)
      y = 0;
    if (c != '\n') {
      terminal_putentryat(c, terminal_color, x, y);
      ++x; // Increment X pos
    }
  }
  terminal_change_cursor_pos(x, y);
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {
  terminal_write(data, strlen(data));
}

void terminal_change_cursor_pos(u8 x, u8 y) {
  u16 position = y * 80 + x;
  outb(video_port, 0x0F);
  outb(video_port + 1, (u8) (position & 0xFF));
  outb(video_port, 0x0E);
  outb(video_port + 1, (u8) ((position >> 8) & 0xFF));
}
