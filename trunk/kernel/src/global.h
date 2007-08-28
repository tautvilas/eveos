#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define KERNEL_CALL     __attribute__((stdcall))

#define DEBUG           1

#if DEBUG
#   define DBG(expr)    { \
                            Out::Dbg() << __FILE__ ":"  << __LINE__ \
                                    << ": " #expr " == "; \
                            __typeof__(expr) _expr_val = (expr); \
                            Out::Dbg() << HEX << _expr_val \
                                     << " (" << DEC << _expr_val << ")\n"; \
                        }
#   define ASSERT(expr) if (0 == (expr)) \
                        { \
                            Out::Dbg() << __FILE__ ":" << __LINE__ \
                                    << ": FAILED ASSERT(" #expr ")\n"; \
                            kernel_stop(); \
                        }
#else
#   define DBG(expr)
#   define ASSERT(expr)
#endif



typedef unsigned int    TSize;

typedef unsigned char   TByte;
typedef unsigned short  TWord;
typedef unsigned int    TDWord;

typedef int             TInt;
typedef unsigned int    TUInt;

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

extern "C" void kernel_stop();


#endif // _GLOBAL_H_

