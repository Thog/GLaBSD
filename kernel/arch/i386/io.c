#include <kernel/i386.h>
#include <kernel/arch.h>
#include <string.h>
#include <stdio.h>


void outb(u16 ad, u8 v) {
  asmv("outb %0, %1" : : "a"(v), "Nd"(ad));
}

void inb(u32 ad) {
  u8 ret;
  asmv("inb %%dx, %%al" : "=a" (ret) : "d" (ad));
  return ret;
}