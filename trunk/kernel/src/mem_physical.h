#ifndef _MEM_PHYSICAL_H_
#define _MEM_PHYSICAL_H_

#include <global.h>
#include <fixed_stack.h>


namespace Mem {


class Physical
{
public:

    enum 
    {
        PAGE_SIZE   = 4 * KILOBYTE,
    };

    static Maybe<Addr> KERNEL_CALL
    alloc();

    static Bool KERNEL_CALL
    dealloc(Addr);

    static Size KERNEL_CALL
    size();

    static Size KERNEL_CALL
    free();

    static Size KERNEL_CALL
    used();

private:

    typedef Generic::FixedStack<Addr>   PageStack;

    static PageStack    mFreePages;

    Physical();

    static void KERNEL_CALL
    init();
    
    friend void Mem::init();
};


};  // namespace Mem

#endif  // _MEM_PHYSICAL_H_
