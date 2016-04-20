#include <stdio.h>

#if defined(__GLABSD_KERNEL__)
#include <kernel/tty.h>
#endif

int putchar(int ic) {
#if defined(__GLABSD_KERNEL__)
  char c = (char) ic;
  terminal_write(&c, sizeof(c));
#else
  // TODO: Write system call.
#endif
  return ic;
}
