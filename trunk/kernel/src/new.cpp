#include <new.h>
#include <mem.h>


void*
operator new(Size size)
{
    // :TODO: 2009-05-03 gx
    return Mem::grow(size).value();
}


void*
operator new(Size size, void* where)
{
    return where;
}


void* 
operator new[](Size size)
{
    // :TODO: 2009-05-03 gx
    return Mem::grow(size).value();
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

