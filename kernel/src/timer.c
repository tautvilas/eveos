#include "timer.h"
#include "stdio.h"
#include "idt.h"
#include "ports.h"
#include "vga.h"
#include "loader.h"

#define PIT_COMMAND  0x43
#define PIT_CHANNEL1 0x40 /* IRQ0 clock */
#define PIT_CHANNEL2 0x41 /* For system */
#define PIT_CHANNEL3 0x42 /* Beep */
#define PIT_RATE     1193180 /* Hz if divided by 1 (default rate = 18.222Hz) */

extern dword_t gKernelEsp;
extern dword_t gLastTaskEsp;

/**
  * PIT IRQ0 handler
  *
  * @param apRegs register information
  */

unsigned static int gTimerTicks = 0U;

static void KERNEL_CALL
timer_handler(regs_t * apRegs)
{
    gTimerTicks++;

    vga_pos_t cursor = vga_get_cursor_pos();
    color_t bg = vga_get_bg_color();
    color_t fg = vga_get_fg_color();

    if(gTimerTicks % _TIMER_RATE == 0)
    {
        vga_set_fg_color(VGA_CL_BLACK);
        vga_set_bg_color(VGA_CL_WHITE);

        int numSecs = gTimerTicks / _TIMER_RATE;
        int length = 0;
        while(numSecs)
        {
            numSecs /= 10;
            length++;
        }
        vga_set_cursor_pos(71 - length, 0); /* TODO magic number to constant */
        printf("Uptime: %ds", gTimerTicks / _TIMER_RATE);
    }

    vga_set_cursor_pos(cursor.mX, cursor.mY);
    vga_set_bg_color(bg);
    vga_set_fg_color(fg);

    if(gTimerTicks % (_TIMER_RATE * 1) == 0 && gpActiveTask != NULL)
    {
        //printf("Runing task %x (parent =  %d, id = %d, esp = %x)\n", gpActiveTask, gpActiveTask->parent, gpActiveTask->id, 
        //        gpActiveTask->esp);
        //DUMP(gLastTaskEsp);
        //DUMP(apRegs->useresp);
    }
    if(gTimerTicks % 100 == 0 && gpActiveTask != NULL)
    {
        //DUMP(gpActiveTaskRingNode);
        //DUMP(gpActiveTaskRingNode->pTask);
        //DUMP(gpActiveTaskRingNode->pNext);
        //DUMP(gpActiveTaskRingNode->pPrev);
        gpActiveTaskRingNode = gpActiveTaskRingNode->pNext;
        gpActiveTask = gpActiveTaskRingNode->pTask;
        //printf("Switched to task %x (parent =  %d, id = %d)\n", gpActiveTask, gpActiveTask->parent, gpActiveTask->id);
    }
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

unsigned int KERNEL_CALL
timer_get_num_ticks()
{
    return gTimerTicks;
}
