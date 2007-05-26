#ifndef _LOADER_H_
#define _LOADER_H_

#include "global.h"
#include "memmgr.h"

/* typedef enum {
    PROC_MODE_SUPERVISOR,
    PROC_MODE_USER
} proc_mode_t; */

void KERNEL_CALL
multitasking_install(void);

void KERNEL_CALL
print_task_tree(void);

void KERNEL_CALL
load_task(void* apOffset, mm_access_t aAccess);

#endif // _LOADER_H_
