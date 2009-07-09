#include <critical.h>


Size    Critical::mDepth    = 0;


/*static*/ bool 
Critical::begin()
{
    if (0 == mDepth)
        __asm__ ("cli");
        
    mDepth++;
    
    return true;
}


/*static*/ bool 
Critical::end()
{
    if (mDepth)
        mDepth--;
        
    if (0 == mDepth)
        __asm__ ("sti");
        
    return false;
}


/*static*/ Size 
Critical::depth()
{
    return mDepth;
}

