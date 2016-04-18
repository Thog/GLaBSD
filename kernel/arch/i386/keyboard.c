#include <kernel/arch.h>
#include <kernel/io.h>

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

}