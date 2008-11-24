#include <mem_physical.h>
#include <mem_physical_page.h>
#include <algorithms.h>

namespace Mem {


Physical::PageStack Physical::mFreePages(0, 0);


/*static*/ void KERNEL_CALL
Physical::init()
{
    using Generic::max;
    using Generic::fill;

    Byte* const SYSTEM_LO_MEM_END   = Kernel::BASE + MEGABYTE;

    Size    base    = reinterpret_cast<Size>(Kernel::BASE);
    Byte*   end     = reinterpret_cast<Byte*>(size());
    Byte*   free    = max(Kernel::END, SYSTEM_LO_MEM_END) - base;
    Size    count   = (end - free) / Page::SIZE;

    ASSERT(free <= end);

    // putting free pages stack low in free memory
    mFreePages  = PageStack(
            reinterpret_cast<PageStack::Iterator>(free + base),
            reinterpret_cast<PageStack::Iterator>(free + base) + count
        );

    // this memory is now used by free pages stack
    free        += count * sizeof(Byte*);

    // filling stack with all awailable free pages
    for (Page p(end - Page::SIZE); p > free; mFreePages.push(p--));
}


/*static*/ Byte* KERNEL_CALL
Physical::alloc()
{
    return mFreePages.pop();
}


/*static*/ void KERNEL_CALL
Physical::dealloc(Byte* addr)
{
    Page        page    = addr;
    PageStack&  stack   = mFreePages;

    ASSERT(page >= reinterpret_cast<Byte*>(stack.end()))
    ASSERT(stack.end() == Generic::find(stack.begin(), stack.end(), page));

    if (page < reinterpret_cast<Byte*>(stack.end()))
        return; // cannot free memory used by kernel and stack

    if (stack.end() != Generic::find(stack.begin(), stack.end(), page))
        return; // page already free

    mFreePages.push(page);
}


/*static*/ Size KERNEL_CALL
Physical::free()
{
    return (mFreePages.end() - mFreePages.begin()) * Page::SIZE;
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
    //      for now assuming that we have 8MB of RAM
    return 1.1 * MEGABYTE;
}


}; // namespace Mem
