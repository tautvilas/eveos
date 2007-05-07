#ifndef _IDT_H_
#define _IDT_H_

#include "global.h"

/**
  * This defines what the stack looks like after an ISR was running
  */
typedef struct
{
    dword_t gs, fs, es, ds;                          /* pushed the segs last */
    dword_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    dword_t int_no, err_code;                        /* push byteX and push ecode */
    dword_t eip, cs, eflags, useresp, ss;            /* pushed by the processor automatically */
} regs_t;

/**
 *  Installs interrupt descriptor table, ISRs and IRQs handling as well
 */

extern void KERNEL_CALL
idt_install(void);

/**
 *  Sets IRQ routine handling function
 *
 *  @param  aIRQ      IRQ number
 *  @param  pHandler  pointer to handling function
 */

extern void KERNEL_CALL
irq_install_handler(int aIrq, void (*handler)(regs_t *apR));

/**
 *  Clears IRQ routine handling function
 *
 *  @param  aIRQ      IRQ number
 */

extern void KERNEL_CALL
irq_uninstall_handler(int aIrq);

/**
 *  Kernel panic - prints out register information and halts
 */

extern void KERNEL_CALL
kernel_panic(void);

#endif  // _IDT_H_
