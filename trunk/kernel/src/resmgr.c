#include "resmgr.h"
#include "global.h"
#include "stdio.h"
#include "loader.h"
#include "keyboard.h"
#include "idt.h"
#include "vga.h"
#include "sem.h"

extern dword_t read_cr3();
extern dword_t write_cr3();

typedef struct {
    task_t* pTask;
    regs_t* pRegs;
    uint_t resource;
} waiting_task_t;

#define MAX_NUM_WAITING_TASKS 32
// can not use malloc here :-|
waiting_task_t gWaitingTasksList[MAX_NUM_WAITING_TASKS];
uint_t gNumWaitingTasks = 0;

void KERNEL_CALL
rm_install(void)
{
    semaphores_install();
    gNumWaitingTasks = 0;
}

void KERNEL_CALL
rm_start(void)
{
    while(1)
    {
        __asm__ __volatile__ ("cli");
        if(gNumWaitingTasks)
        {
            int id = gNumWaitingTasks - 1;
            uint_t resource = gWaitingTasksList[id].resource;
            if(resource == KEYBOARD_SEMAPHORE)
            {
                if(semaphore_get(KEYBOARD_SEMAPHORE))
                {
                    semaphore_p(KEYBOARD_SEMAPHORE);
                    task_t* pTask = gWaitingTasksList[id].pTask;
                    regs_t* pRegs = gWaitingTasksList[id].pRegs;
                    semaphore_v(KEYBOARD_SEMAPHORE);
                    if (keyboard_had_input())
                    {
                        dword_t kernelPageDir = read_cr3();
                        dword_t taskPageDir = pTask->page_dir;

                        write_cr3(taskPageDir);

                        uint_t numbytes = pRegs->edx;
                        byte_t* pBuffer = (byte_t*) pRegs->ecx;
                        uint_t bytes_read = 0;

                        while (keyboard_had_input() && numbytes)
                        {
                            char c = keyboard_getchar();
                            if (c >31 || c == '\n' || c == '\b')
                            {
                                vga_print_char(c);
                                numbytes--;
                                bytes_read++;
                                *pBuffer = c;
                                pBuffer++;
                                c = keyboard_getchar();
                            }
                        }

                        if (!numbytes)
                        {
                            pRegs->eax = bytes_read;
                            gNumWaitingTasks--;
                            pTask->locked = FALSE;
                        }
                        else
                        {
                            pRegs->ecx = (dword_t) pBuffer;
                        }
                        write_cr3(kernelPageDir);
                    }
                }
            }
        }
        __asm__ __volatile__ ("sti");
    }
}

void KERNEL_CALL
rm_add_waiting_task(task_t* apTask, regs_t* apRegs, uint_t aResource)
{
    __asm__ __volatile__ ("cli");
    gWaitingTasksList[gNumWaitingTasks].pTask = apTask;
    gWaitingTasksList[gNumWaitingTasks].pRegs = apRegs;
    gWaitingTasksList[gNumWaitingTasks].resource = aResource;
    gNumWaitingTasks++;
    if(gNumWaitingTasks >= 32)
    {
        printf("Waiting tasks stack is full!");
        kernel_panic();
    }
}
