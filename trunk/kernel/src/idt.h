#ifndef _IDT_H_
#define _IDT_H_

#include "global.h"

/**
 *  Installs interrupt descriptor table and ISRs
 */

extern void KERNEL_CALL
idt_install();

#endif  // _IDT_H_
