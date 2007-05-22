#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include "sys_write.h"

enum syscalls_t {
    SYS_EXIT = 1,
    SYS_FORK,
    SYS_READ,
    SYS_WRITE,
    SYS_OPEN,
    SYS_CLOSE
}

#endif // _SYSCALLS_H_
