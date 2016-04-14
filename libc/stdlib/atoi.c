#include <stdio.h>

int atoi(const char *s) {
  int i;
  int n;
  int sign;

  i = 0;
  n = 0;
  sign = 1;
  while ((s[i] == ' ') || (s[i] == '\t') || (s[i] == '\n')
      || (s[i] == '\v') || (s[i] == '\f') || (s[i] == '\r'))
    i++;
  sign = (s[i] == '-') ? -1 : 1;
  i = (s[i] == '-' || s[i] == '+') ? i + 1 : i;
  while (s[i] && s[i] >= '0' && s[i] <= '9') {
    n = n * 10 + (s[i] - '0');
    i++;
  }
  return (n * sign);
}
