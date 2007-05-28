#include "global.h"
#include "idt.h"
#include "vga.h"
#include "stdio.h"
#include "sys_write.h"
#include "mem.h"

#define STDOUT 1

void KERNEL_CALL
sys_write(regs_t* apRegs)
{
    dword_t file_descriptor = apRegs->ebx;
    char * offset = (char*) apRegs->ecx;
    dword_t length = apRegs->edx;

    //memdump((byte_t*)offset, 10);
    //DUMP(length);
    //DUMP(offset);
    //STOP;

    //DUMP(apRegs->ecx);
    //DUMP(apRegs->ebp);
    //DUMP(apRegs->esp);

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
