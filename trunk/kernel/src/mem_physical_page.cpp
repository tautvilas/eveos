#include <mem_physical_page.h>

namespace Mem {


KERNEL_CALL
Physical::Page::Page() : mAddr(0)
{}


KERNEL_CALL
Physical::Page::Page(Byte* addr)
        : mAddr(addr - reinterpret_cast<Size>(addr) % SIZE)
{}


KERNEL_CALL
Physical::Page::operator Byte*() const
{
    return mAddr;
}


KERNEL_CALL Physical::Page&
Physical::Page::operator ++()
{
    mAddr += SIZE;
    return *this;
}


KERNEL_CALL Physical::Page
Physical::Page::operator ++(int)
{
    Page    p(mAddr);
    mAddr += SIZE;
    return p;
}


KERNEL_CALL Physical::Page&
Physical::Page::operator --()
{
    mAddr -= SIZE;
    return *this;
}


KERNEL_CALL Physical::Page
Physical::Page::operator --(int)
{
    Page    p(mAddr);
    mAddr -= SIZE;
    return p;
}


}; // namespace Mem
