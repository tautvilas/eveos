#include "timer.h"
#include "stdio.h"
#include "idt.h"
#include "ports.h"

#define PIT_COMMAND  0x43
#define PIT_CHANNEL1 0x40 /* IRQ0 clock */
#define PIT_CHANNEL2 0x41 /* For system */
#define PIT_CHANNEL3 0x42 /* Beep */
#define PIT_RATE     1193180 /* Hz if divided by 1 (default rate = 18.222Hz) */

/**
  * PIT IRQ0 handler
  *
  * @param apRegs register information
  */

unsigned int gTimerTicks = 0U;

static void KERNEL_CALL
timer_handler(regs_t * apRegs)
{
    gTimerTicks++;
    /*
    if(gTimerTicks % _TIMER_RATE == 0) {
        printf("One second passed\n");
    }
    */
    return;
}

/* set how many timer per second irq0 will fire */

static void KERNEL_CALL
timer_set_rate(int aHz)
{
    int divisor = PIT_RATE / aHz;            /* Calculate our divisor */
    outportb(PIT_COMMAND, 0x36);             /* Set our command byte 0x36 */
    outportb(PIT_CHANNEL1, divisor & 0xFF);  /* Set low byte of divisor */
    outportb(PIT_CHANNEL1, divisor >> 8);    /* Set high byte of divisor */
}

void KERNEL_CALL
timer_install()
{
    timer_set_rate(_TIMER_RATE);
    irq_install_handler(0, timer_handler);
}
