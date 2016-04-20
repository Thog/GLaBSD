#include <kernel/arch.h>
#include <kernel/i386.h>

inline void outb(u16 address, u8 value) {
  asmv("outb %0, %1" : : "a"(value), "Nd"(address));
}

inline void outw(u16 address, u16 value) {
  asmv("outw %0, %1" : : "a"(value), "Nd"(address));
}

inline void outl(u16 address, u32 value) {
  asmv("outl %0, %1" : : "a"(value), "Nd"(address));
}

inline u8 inb(u16 address) {
  u8 ret;
  asmv("inb %1, %0" : "=a" (ret) : "Nd" (address));
  return ret;
}

inline u16 inw(u16 address) {
  u16 ret;
  asmv("inw %1, %0" : "=a" (ret) : "Nd" (address));
  return ret;
}

inline u32 inl(u16 address) {
  u32 ret;
  asmv("inl %1, %0" : "=a" (ret) : "Nd" (address));
  return ret;
}

inline void io_wait(void) {
  asmv("outb %0, $0x80" : : "a"(0));
}

void send_eoi(u8 irq) {
  if (irq >= 8)
    outb(PIC2_COMMAND, PIC_EOI);

  outb(PIC1_COMMAND, PIC_EOI);
}

u16 get_total_ram(void) {
  unsigned char low, high;

  outb(0x70, 0x30);
  low = inb(0x71);
  outb(0x70, 0x31);
  high = inb(0x71);
  return low | high << 8;
}