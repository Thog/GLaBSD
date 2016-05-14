#ifndef _KERNEL_TASKS_H
#define _KERNEL_TASKS_H

#include <kernel/i386.h>

void jump_to_task(tss task);
#endif
