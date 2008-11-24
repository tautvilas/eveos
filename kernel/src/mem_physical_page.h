#ifndef _MEM_PHYSICAL_PAGE_H_
#define _MEM_PHYSICAL_PAGE_H_

#include <mem_physical.h>

namespace Mem {


class Physical::Page
{
public:
    enum
    {
        SIZE    = 4 * KILOBYTE,
    };

    KERNEL_CALL
    Page();

    KERNEL_CALL
    Page(Byte* addr);

    KERNEL_CALL
    operator Byte*() const;

    Page& KERNEL_CALL
    operator ++();

    Page KERNEL_CALL
    operator ++(int);

    Page& KERNEL_CALL
    operator --();

    Page KERNEL_CALL
    operator --(int);


private:
    Byte*   mAddr;
};


};  // namespace Mem

#endif  // _MEM_PHYSICAL_PAGE_H_
