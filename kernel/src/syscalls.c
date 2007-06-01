#include "syscalls.h"
#include "keyboard.h"
#include "global.h"
#include "stdio.h"
#include "sem.h"
#include "resmgr.h"
#include "vga.h"
#include "loader.h"
#include "timer.h"

#define STDIN  0
#define STDOUT 1

void KERNEL_CALL
sys_read(regs_t* apRegs)
{
    //DUMP(apRegs->ecx);
    //DUMP(apRegs->ebx);
    //DUMP(apRegs->edx);
    dword_t file_descriptor = apRegs->ebx;
    //char * offset = (char*) apRegs->ecx;
    //dword_t numbytes = apRegs->edx;

    if (file_descriptor == STDIN && !gpActiveTask->locked)
    {
        rm_add_waiting_task(gpActiveTask, apRegs, KEYBOARD_SEMAPHORE);
        gpActiveTask->locked = TRUE;
        timer_schedule(TRUE);
    }
    return;
}

void KERNEL_CALL
sys_write(regs_t* apRegs)
{
    dword_t file_descriptor = apRegs->ebx;
    char * offset = (char*) apRegs->ecx;
    dword_t length = apRegs->edx;

    if (file_descriptor == STDOUT)
    {
        int i;
        for(i = 0; i < length; i++, offset++)
        {
            vga_print_char(*offset);
        }
    }
    return;
}

void KERNEL_CALL
sys_execve(regs_t* apRegs)
{
    // priority_t priority = apRegs->ebx;
    // char * name = (char*) apRegs->ecx;
    // bool_t background = apRegs->edx;
    return;
}
