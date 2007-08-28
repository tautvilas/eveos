#ifndef _OUT_STREAM_H_
#define _OUT_STREAM_H_

#include "stream.h"
#include "vga.h"


class TOutStream
        : public TStream
{
public:

    TOutStream& KERNEL_CALL
    operator <<(const char*);

    TOutStream& KERNEL_CALL
    operator <<(char);

    TOutStream& KERNEL_CALL
    operator <<(int);

    TOutStream& KERNEL_CALL
    operator <<(unsigned int);

    TOutStream& KERNEL_CALL
    operator <<(TIntegerBase);


private:



};



////// TOutStream inlines //////

inline TOutStream& KERNEL_CALL
TOutStream::operator <<(const char* apStr)
{
    Vga::Print(apStr);
    return *this;
}


inline TOutStream& KERNEL_CALL
TOutStream::operator <<(char aChar)
{
    Vga::Print(aChar);
    return *this;
}


inline TOutStream& KERNEL_CALL
TOutStream::operator <<(int aInt)
{
    Vga::Print(IntToStr(aInt, IntBase()));
    return *this;
}


inline TOutStream& KERNEL_CALL
TOutStream::operator <<(unsigned int aInt)
{
    Vga::Print(IntToStr(aInt, IntBase(), false));
    return *this;
}


inline TOutStream& KERNEL_CALL
TOutStream::operator <<(TIntegerBase aBase)
{
    IntBase(aBase);
    return *this;
}


#endif // _OUT_STREAM_H_

