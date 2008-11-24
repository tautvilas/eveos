#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define KERNEL_CALL     __attribute__((stdcall))

#define NULL            0

typedef unsigned char   Byte;
typedef unsigned short  Word;
typedef unsigned int    DWord;

typedef unsigned int    Size;       // always the same size as void*
typedef unsigned int    Index;

typedef int             Int;        // always the same size as void*
typedef unsigned int    UInt;       // always the same size as void*
typedef bool            Bool;


enum IntegerBase
{
    BIN     = 2,
    OCT     = 8,
    DEC     = 10,
    HEX     = 16,
};

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


struct Kernel
{
    static Byte* const  BASE;
    static Byte* const  START;
    static Byte* const  END;
    static const Index  OFFSET;
};


extern "C" void kernel_stop();

#endif // _GLOBAL_H_
