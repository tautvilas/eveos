#ifndef _MEM_H_
#define _MEM_H_

#include <global.h>

namespace Mem {    
    

void 
init();

Maybe<void*> 
grow(Size size);

Size 
used();


}

#endif
