#ifndef _IDT_H_
#define _IDT_H_

#include "global.h"

class Idt
{
public:
    static void KERNEL_CALL
    Install();
private:
};

////// Idt inlines //////

#endif // _IDT_H_

