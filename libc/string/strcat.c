#include <string.h>

char *strcat(char *s1, const char *s2) {
  char *ret;

  ret = s1;
  while (*s1)
    s1++;
  while ((*s1++ = *s2++));
  return (ret);
}