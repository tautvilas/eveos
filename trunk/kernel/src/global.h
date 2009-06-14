#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define KERNEL_CALL     __attribute__((stdcall))


#include <types.h>
#include <maybe.h>


struct Kernel
{
    static const Addr   BASE;
    static const Addr   START;
    static const Addr   END;
};


extern "C" void kernel_stop();


inline Addr 
addr(void* ptr)
{
    return reinterpret_cast<Addr>(ptr);
}

template<typename Ptr>
inline Ptr 
addr_cast(Addr addr)
{
    return reinterpret_cast<Ptr>(addr);
}


#endif // _GLOBAL_H_
