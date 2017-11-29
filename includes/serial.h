#ifndef SERIAL_H
#define SERIAL_H

#include "types.h"
#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8


void init_serial(u16 port);
char read_serial(u16 port);
void write_serial(u16 port, char c);

void serial_printkc(char c);
void serial_printk(char *str);

#endif