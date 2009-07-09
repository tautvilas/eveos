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

    static Maybe<Addr> 
    alloc();

    static Bool 
    dealloc(Addr);

    static Size 
    size();

    static Size 
    free();

    static Size 
    used();

private:

    typedef Generic::FixedStack<Addr>   PageStack;

    static PageStack    mFreePages;

    Physical();

    static void 
    init();
    
    friend void Mem::init();
};


};  // namespace Mem

#endif  // _MEM_PHYSICAL_H_
