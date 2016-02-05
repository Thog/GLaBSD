#include <string.h>

size_t strlen (const char *s)
{
  size_t i;
  unsigned long long hack;

  i = 0;
  while (1)
    {
      hack = *((unsigned long long *) (s + i));
      if (!(hack & 0xFF))
        return (i);
      if (!(hack & 0xFF00))
        return (i + 1);
      if (!(hack & 0xFF0000))
        return (i + 2);
      if (!(hack & 0xFF000000))
        return (i + 3);
      if (!(hack & 0xFF00000000))
        return (i + 4);
      if (!(hack & 0xFF0000000000))
        return (i + 5);
      if (!(hack & 0xFF000000000000))
        return (i + 6);
      if (!(hack & 0xFF00000000000000))
        return (i + 7);
      i += 8;
    }
}
