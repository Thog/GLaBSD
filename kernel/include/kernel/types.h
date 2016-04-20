#ifndef _KERNEL_TYPES_H
#define _KERNEL_TYPES_H

typedef unsigned char u_char; // 8 bits unsigned
typedef unsigned char u8; // 8 bits unsigned
typedef unsigned short u16; // 16 bits unsigned
typedef unsigned int u32; // 32 bits unsigned
typedef unsigned long long u64; // 64 bits unsigned

typedef signed char s8; // 8 bits signed
typedef signed short s16; // 16 bits signed
typedef signed int s32; // 32 bits signed
typedef signed long long s64; // 64 bits signed

// GRUB Structure
typedef struct multiboot_memory_map {
  u32 size, base_addr_low, base_addr_high, length_low, length_high, type;
} multiboot_memory_map_t;

#define true 1
#define false 0

#endif //_KERNEL_TYPES_H
