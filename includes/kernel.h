#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"
#include "string.h"
#include "boot.h"
#include "pic.h"
#include "gdt.h"
#include "screen.h"
#include "serial.h"
#include "logging.h"
#include "debugging.h"

void *get_kernel_base(void);
void set_kernel_virtual_start(void *addr);

void memory_managment_init(void);
#endif