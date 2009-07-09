#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <types.h>


struct Kernel
{
    static const Addr   BASE;
    static const Addr   START;
    static const Addr   END;
};


extern "C" void kernel_stop();


#endif  // _KERNEL_H_
