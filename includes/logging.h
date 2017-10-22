#ifndef LOGGING_H
#define LOGGING_H

#define BASE_2 "01"
#define BASE_8 "01234567"
#define BASE_10 "0123456789"
#define BASE_16 "0123456789ABCDEF"

void printk(char *str);
void printkc(char c);
void printk_int(u32 data, char *base);

#endif