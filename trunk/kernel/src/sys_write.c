#include "global.h"
#include "idt.h"
#include "vga.h"
#include "stdio.h"
#include "sys_write.h"

#define STDOUT 1

void KERNEL_CALL
sys_write(regs_t* apRegs)
{
    DBG_DUMP(1000);
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
