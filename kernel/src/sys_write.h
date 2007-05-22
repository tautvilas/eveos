#ifndef _SYS_WRITE_H_
#define _SYS_WRITE_H_

#include "global.h"
#include "idt.h"

extern void KERNEL_CALL
sys_write(regs_t* apRegs);

#endif // _SYS_WRITE_H_
