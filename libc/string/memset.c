#include <string.h>

void *memset(void *ptr, int value, size_t len) {
  unsigned char *str;

  str = (unsigned char *) ptr;
  while (len--)
    *str++ = value;
  return (ptr);
}
