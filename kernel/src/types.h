#ifndef _TYPES_H_
#define _TYPES_H_


#include <maybe.h>

#define EXTERN_CALL     __attribute__((stdcall))

typedef unsigned char   Byte;
typedef unsigned short  Word;
typedef unsigned int    DWord;

typedef unsigned int    Size;       // always the same size as void*
typedef unsigned int    Index;      // always the same size as void*
typedef unsigned int    Addr;       // always the same size as void*

typedef int             Int;        // always the same size as void*
typedef unsigned int    UInt;       // always the same size as void*
typedef bool            Bool;


enum BitsPerByte
{
    BITS_PER_BYTE   = 8
};

enum MemoryMeasure
{
    BYTE            = 1,
    KILOBYTE        = BYTE * 1024,
    MEGABYTE        = KILOBYTE * 1024,
    GIGABYTE        = MEGABYTE * 1024,
};


inline Addr 
addr(const void* ptr)
{
    return reinterpret_cast<Addr>(ptr);
}

template<typename Ptr>
inline Ptr 
addr_cast(Addr addr)
{
    return reinterpret_cast<Ptr>(addr);
}

#endif

