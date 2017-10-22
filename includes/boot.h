#ifndef BOOT_H
#define BOOT_H

typedef struct multiboot2_s
{
    u16 type;
    u16 flags;
    u32 size;
} multiboot2_t;

static inline void outb(u16 port, u8 val)
{
    asm volatile("outb %0, %1" :: "a"(val), "Nd"(port));
}

static inline void outw(u16 port, u16 val)
{
    asm volatile("outw %0, %1" :: "a"(val), "Nd"(port));
}

static inline void outl(u16 port, u32 val)
{
    asm volatile("outl %0, %1" :: "a"(val), "Nd"(port));
}

static inline void io_wait(void)
{
    // Linux kernel is doing this like that :/
    outb(0x80, 0);
}

static inline void outb_p(u16 port, u8 val)
{
    outb(port, val);
    io_wait();
}

static inline u8 inb(u16 port)
{
    u8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline u16 inw(u16 port)
{
    u16 ret;
    asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline u32 inl(u16 port)
{
    u32 ret;
    asm volatile("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#endif