#include "kernel.h"

void init_serial(u16 port)
{
    // Disable all interrupts
    outb(port + 1, 0x00);
    // Enable DLAB (set baud rate divisor)
    outb(port + 3, 0x80);
    // Set divisor to 3 (lo byte)
    outb(port, 0x03);
    // Set divisor to 3 (hi byte)
    outb(port + 1, 0x00);
    // 8 bits, no parity, one stop bit
    outb(port + 3, 0x03);
    // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 2, 0xC7);
    // IRQs enabled, RTS/DSR set
    outb(port + 4, 0x0B);
}

static int has_received_data(u16 port)
{
    return inb(port + 5) & 1;
}
 
char read_serial(u16 port)
{
    while (!has_received_data(port));
    return inb(port);
}

static int is_transport_empty(u16 port)
{
    return inb(port + 5) & 0x20;
}
 
void write_serial(u16 port, char c)
{
    while (!is_transport_empty(port));
    outb(port, c);
}