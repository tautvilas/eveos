#ifndef _MEM_PHYSICAL_H_
#define _MEM_PHYSICAL_H_

#include <global.h>
#include <fixed_stack.h>


namespace Mem {


class Physical
{
public:

    static void KERNEL_CALL
    init();

    static Byte* KERNEL_CALL
    alloc();

    static void KERNEL_CALL
    dealloc(Byte*);

    static Size KERNEL_CALL
    size();

    static Size KERNEL_CALL
    free();

    static Size KERNEL_CALL
    used();

private:

    class Page;

    typedef Generic::FixedStack<Byte*>  PageStack;

    static PageStack    mFreePages;

    Physical();
};


};  // namespace Mem

#endif  // _MEM_PHYSICAL_H_
