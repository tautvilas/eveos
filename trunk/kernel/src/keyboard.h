#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "global.h"

/* install ps2 keyboard input handler */

extern void KERNEL_CALL
keyboard_install(void);

/* enable A20 gate (memory past 1MB) */

extern void KERNEL_CALL
a20_enable(void);

#endif // _KEYBOARD_H_
