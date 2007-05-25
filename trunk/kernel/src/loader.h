#ifndef _LOADER_H_
#define _LOADER_H_

#include "global.h"

typedef enum {
    PROC_MODE_SUPERVISOR,
    PROC_MODE_USER
} proc_mode_t;

extern void KERNEL_CALL
multitasking_install(void);

extern void KERNEL_CALL
print_process_tree(void);

extern void KERNEL_CALL
load_task(void* apOffset, proc_mode_t aMode);

#endif // _LOADER_H_
