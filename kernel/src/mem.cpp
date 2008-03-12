#include "mem.h"
#include "fixed_stack.h"
#include "algorithms.h"

using namespace Generic;


/*static*/ Mem::PageStack   Mem::msFreePages(0, 0);

/*static*/ void KERNEL_CALL
Mem::init()
{
    const Size SYSTEM_LO_MEM_SIZE   = MEGABYTE;

    // :TODO: gd 8/29/2007: get real RAM size from BIOS
    //      for now assuming that we have 8MB of RAM
    Size    mem_size    = MEGABYTE * 8;
    Size    used_mem    = max(
            static_cast<Size>(&gKernelEnd - &gKernelBase),
            SYSTEM_LO_MEM_SIZE
        );

    Size    stack_size  = (mem_size - used_mem) / TPage::SIZE;
    TPage*  pStackBegin = reinterpret_cast<TPage*>(&gKernelBase + used_mem);

    // puting free pages stack just after used memory
    // we're wasting few bytes here as stack itself will never be freed
    // but we're allocating memory for page addresses used by it
    msFreePages = PageStack(pStackBegin, pStackBegin + stack_size);

    // adding free pages stack to used memory
    used_mem    += stack_size * sizeof(TPage*);

    for (Size addr = mem_size - TPage::SIZE;
            addr > used_mem;    addr -= TPage::SIZE)
        msFreePages.push(reinterpret_cast<Byte*>(addr));


    //Out::Dbg() << msFreePages;
    //Paging::Init(used);
}
