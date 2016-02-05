#include <string.h>

void *memcpy (void *restrict s1, const void *restrict s2, size_t n)
{
  size_t i;

  i = 0;
  while (n > i)
    {
      ((char *) s1)[i] = ((char *) s2)[i];
      i++;
    }
  return (s1);
}
