#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"
#include "string.h"

#include "arch/boot.h"
#include "arch/gdt.h"
#include "arch/debugging.h"
#include "drivers/pic.h"
#include "drivers/vga_text_mode.h"
#include "drivers/serial.h"

#include "kernel/logging.h"

void *get_kernel_base(void);
void set_kernel_virtual_start(void *addr);

void memory_managment_init(void);
#endif