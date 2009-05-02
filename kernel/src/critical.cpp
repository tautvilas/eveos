#include <critical.h>


Size    Critical::mDepth    = 0;


/*static*/ bool KERNEL_CALL
Critical::begin()
{
    if (0 == mDepth)
        __asm__ ("cli");
        
    mDepth++;
    
    return true;
}


/*static*/ bool KERNEL_CALL
Critical::end()
{
    if (mDepth)
        mDepth--;
        
    if (0 == mDepth)
        __asm__ ("sti");
        
    return false;
}


/*static*/ Size KERNEL_CALL
Critical::depth()
{
    return mDepth;
}

