#ifndef LOGGING_H
#define LOGGING_H

#define BASE_2 "01"
#define BASE_8 "01234567"
#define BASE_10 "0123456789"
#define BASE_16 "0123456789ABCDEF"

#ifdef SERIAL_LOGGING
#define printk serial_printk
#define printkc serial_printkc
#else
#define printk screen_printk
#define printkc screen_printkc
#endif

void printk_int(u64 data, char *base);

#endif