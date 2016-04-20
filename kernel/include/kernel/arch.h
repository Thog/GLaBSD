#ifndef _KERNEL_ARCH_H
#define _KERNEL_ARCH_H
#define asm        __asm__
#define asmv        __asm__ __volatile__
#include <kernel/types.h>
#include <kernel/grub.h>

void init_arch(void);
void set_arch_param(u32 ret, u32 ret1, u32 ret2, u32 ret3, u32 ret4);
u32 get_arch_arg(u32 n);
void kernel_panic(char *error);

multiboot_info_t *get_multiboot_info(void);
void set_multiboot_info(multiboot_info_t *mbt);

/**
 * Get total RAM with GRUB multiboot info
 */
u64 get_total_ram(void);

#endif //_KERNEL_ARCH_H
