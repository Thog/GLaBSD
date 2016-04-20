#ifndef _KERNEL_IO_H
#define _KERNEL_IO_H
#include <kernel/types.h>
#define ICW1_ICW4    0x01        /* ICW4 (not) needed */
#define ICW1_SINGLE    0x02        /* Single (cascade) mode */
#define ICW1_INTERVAL4    0x04        /* Call address interval 4 (8) */
#define ICW1_LEVEL    0x08        /* Level triggered (edge) mode */
#define ICW1_INIT    0x10        /* Initialization - required! */

#define ICW4_8086    0x01        /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO    0x02        /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE    0x08        /* Buffered mode/slave */
#define ICW4_BUF_MASTER    0x0C        /* Buffered mode/master */
#define ICW4_SFNM    0x10        /* Special fully nested (not) */

#define PIC1        0x20        /* IO base address for master PIC */
#define PIC2        0xA0        /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA    (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA    (PIC2+1)
#define PIC_EOI        0x20        /* End-of-interrupt command code */

/**
 * Sends a 8-bit value on a I/O location
 */
void outb(u16 address, u8 value);

/**
 * Sends a 16-bit value on a I/O location
 */
void outw(u16 address, u16 value);

/**
 * Sends a 32-bit value on a I/O location
 */
void outl(u16 address, u32 value);


/**
 * Receives a 8-bit value from an I/O location
 */
u8 inb(u16 address);

/**
 * Receives a 16-bit value from an I/O location
 */
u16 inw(u16 address);

/**
 * Receives a 32-bit value from an I/O location
 */
u32 inl(u16 address);

/**
 * Forces the CPU to wait for an I/O operation to complete.
 */
void io_wait(void);

/**
 * Send a EOI (End of Interrupt) command to the PIC chips.
 */
void send_eoi(u8 irq);

/**
 * Get RAM total size using CMOS (in byte)
 */
u16 get_total_ram(void);

#endif