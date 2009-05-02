#ifndef _MEM_H_
#define _MEM_H_

#include <global.h>

namespace Mem {    
    

void KERNEL_CALL
init();

void* KERNEL_CALL
grow(Size size);

Size KERNEL_CALL
used();


}

#endif
