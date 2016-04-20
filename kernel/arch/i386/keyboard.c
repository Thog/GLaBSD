#include <kernel/arch.h>
#include <kernel/io.h>
#include <stdio.h>

char getScancode() {
  char c = 0;
  do {
    if (inb(0x60) != c) {
      c = inb(0x60);
      if (c > 0)
        return c;
    }
  } while (1);
}

void isr_kbd_int(void) {
  char key = getScancode();
  printf("Key pressed! Scancode: %i\n", key);
  send_eoi(1);
}