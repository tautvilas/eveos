#include <mem_physical.h>
#include <algorithms.h>

namespace Mem {


Physical::PageStack Physical::mFreePages;


namespace {

    Addr KERNEL_CALL
    page(Addr addr)
    {
        return addr - (addr % Physical::PAGE_SIZE);
    }
    
}


/*static*/ void KERNEL_CALL
Physical::init()
{
    Addr    SYS_LO_MEM  = Kernel::BASE + MEGABYTE;
    
    Addr    end         = size();
    Addr    stackAddr   = Generic::max(Kernel::END, SYS_LO_MEM);
    Addr    firstFree   = stackAddr - Kernel::BASE;

    ASSERT(firstFree <= end);

    // putting free pages stack low in free memory
    Size    stackSize   = (end - firstFree) / PAGE_SIZE;
    mFreePages          = PageStack(addr_cast<Addr*>(stackAddr), stackSize);

    // this memory is now used by free pages stack
    firstFree           += stackSize * sizeof(Addr);

    // filling stack with all available free pages
    for (Addr p = page(end - PAGE_SIZE); p > firstFree; mFreePages.push(p))
        p   -= PAGE_SIZE;
}


/*static*/ Addr KERNEL_CALL
Physical::alloc()
{
    if (!mFreePages.empty())
        return mFreePages.pop();
    else
        return NULL;
}


/*static*/ Bool KERNEL_CALL
Physical::dealloc(Addr pg)
{
    Addr        p       = page(pg);
    PageStack&  stack   = mFreePages;

    ASSERT(p >= addr(stack.end()))
    ASSERT(stack.end() == Generic::find(stack.begin(), stack.end(), p));

    if (p < addr(stack.end()))
        return false;   // cannot free memory used by kernel and stack

    if (stack.end() != Generic::find(stack.begin(), stack.end(), p))
        return false;   // page already free

    mFreePages.push(p);
    return true;
}


/*static*/ Size KERNEL_CALL
Physical::free()
{
    return mFreePages.size() * PAGE_SIZE;
}


/*static*/ Size KERNEL_CALL
Physical::used()
{
    return size() - free();
}


/*static*/ Size KERNEL_CALL
Physical::size()
{
    // :TODO: gd 2008-11-24: get real RAM size from BIOS
    //      for now assuming that we have 2MB of RAM
    return 2 * MEGABYTE;
}


}; // namespace Mem
