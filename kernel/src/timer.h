#ifndef _TIMER_H_
#define _TIMER_H_

#include "global.h"

#define _TIMER_RATE   1000 /* IRQ0 will be firing 1000 timer per second */

extern unsigned int gTimerTicks;

/* install PIR IRQ0 handler and set PIT firing rate */

extern void KERNEL_CALL
timer_install(void);

#endif // _TIMER_H_
