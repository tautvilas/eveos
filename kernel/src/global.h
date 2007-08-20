#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define KERNEL_CALL __attribute__((stdcall))

typedef unsigned int    TSize;

typedef unsigned char   TByte;
typedef unsigned short  TWord;
typedef unsigned int    TDWord;

enum TIntegerBase
{
    BIN     = 2,
    OCT     = 8,
    DEC     = 10,
    HEX     = 16,
};

enum TBitsPerByte
{
    BITS_PER_BYTE   = 8
};

extern TByte gKernelBase;


#endif // _GLOBAL_H_

