#include <kernel/arch.h>
#include <kernel/keyboard.h>
#include <stdio.h>

u8 shift_locked = 0;

u8 us_keyboard[] = {
    '\0', 27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r',   /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* Enter key */
    '\0',          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
    '\'', '`', '\0',        /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
    'm', ',', '.', '/', '\0',                  /* Right shift */
    '*'};
u8 us_keyboard_upper[] = {
    '\0', '\0', '!', '"', '$', '$', '%', '^', '&', '*',   /* 9 */
    '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R',
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',      /* Enter key */
    '\0',         /* 29   - Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',   /* 39 */
    '|', '~', '\0',
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N',         /* 49 */
    'M', '<', '>', '?', '\0',                  /* Right shift */
    '\0'};

u8 get_scancode() {
  return inb(0x60);
}

u8 scancode_to_ascii(u8 key) {
  if (key < 0x38)
    return shift_locked ? us_keyboard_upper[key] : us_keyboard[key];
  if (key == 0x39)
    return ' ';
  return 0;
}

void keyboard_ack(void) {
  while (inb(0x60) != 0xFA);
}

void keyboard_led(u8 status)
{
  outb(0x60, 0xED);
  keyboard_ack();
  outb(0x60, status);
}

void isr_kbd_int(void) {
  u8 key = get_scancode();

  if (key & 0x80) {
    if (key == 170)
      shift_locked = 0;
  }
  else {
    if (key == 42) {
      shift_locked = 1;
    }
    else if (key == 58)
      shift_locked = !shift_locked;
    else {
      u8 c = scancode_to_ascii(key);
      if (c)
        putchar(c);
    }

  }
  send_eoi(1);
}

void init_keyboard(void) {

}