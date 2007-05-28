#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include "global.h"

extern void KERNEL_CALL
sys_read(regs_t* apRegs);

extern void KERNEL_CALL
sys_write(regs_t* apRegs);

enum syscalls_t {
    SYS_EXIT = 1,
    SYS_FORK,
    SYS_READ,
    SYS_WRITE,
    SYS_OPEN,
    SYS_CLOSE
};

#endif // _SYSCALLS_H_
