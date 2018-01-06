#include "kernel.h"

void remap_interrupts(void)
{
    u8 pic1_data;
    u8 pic2_data;

    // Save masks
    pic1_data = inb(PIC1_DATA);
    pic2_data = inb(PIC2_DATA);

    // ICW1
    outb_p(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb_p(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // ICW2
    outb_p(PIC1_DATA, 0x20); // remap to 0x20 the master vector
    outb_p(PIC2_DATA, 0x70); // remap to 0x70 the slave vector

    // ICW3 (Master/Slave)
    outb_p(PIC1_DATA, 4); // Tell the master that a slave is here with ID 2 (0000 0100)
    outb_p(PIC2_DATA, 2); // Tell the slave his id

    // ICW4
    outb_p(PIC1_DATA, ICW4_8086);
    outb_p(PIC2_DATA, ICW4_8086);

    // Restore masks
    outb(PIC1_DATA, pic1_data);
    outb(PIC2_DATA, pic2_data);
    asm volatile ("sti");
}

void mask_interrupts(void)
{
    outb(PIC1_DATA, 0xFB); // mask everything except cascade on the master
    outb(PIC2_DATA, 0xFF); // mask everything on the slave
}