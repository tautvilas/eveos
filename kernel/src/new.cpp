#include <new.h>
#include <mem.h>


void*
operator new(Size size)
{
    // :TODO: 2009-05-03 gx
    return Mem::grow(size);
}


void* 
operator new[](Size size)
{
    // :TODO: 2009-05-03 gx
    return Mem::grow(size);
}


void 
operator delete(void* p)
{
    // :TODO: 2009-05-03 gx
}


void 
operator delete[](void* p)
{
    // :TODO: 2009-05-03 gx
}

