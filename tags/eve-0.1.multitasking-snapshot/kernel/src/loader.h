#ifndef _LOADER_H_
#define _LOADER_H_

#include "global.h"
#include "memmgr.h"

/* WARNING!!! if YOU change this struct you also mus change common interrupt handlers */
typedef struct {
    dword_t         esp;    //actual position of esp
    size_t          id;
    dword_t         page_dir;
    dword_t         ss;     //actual stack segment.
    dword_t         kstack; //stacktop of kernel stack
    dword_t         ustack; //stacktop of user stack
    dword_t         locked;
    size_t          parent;
    size_t          timetorun;
    size_t          sleep;
    size_t          priority;
    mm_task_mem_t   vm_info;
} task_t;

typedef struct task_ring_node_t {
    task_t*                     pTask;
    struct task_ring_node_t*    pNext;
    struct task_ring_node_t*    pPrev;
} task_ring_node_t;

void KERNEL_CALL
multitasking_install(void);

void KERNEL_CALL
print_task_tree(void);

void KERNEL_CALL
load_task(void* apOffset, mm_access_t aAccess);

extern task_t* gpActiveTask;
extern task_ring_node_t* gpActiveTaskRingNode;
extern dword_t gNextTaskOffset;
extern dword_t gKernelCr3;

#endif // _LOADER_H_
