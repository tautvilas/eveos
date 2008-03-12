#ifndef _OUT_STREAM_H_
#define _OUT_STREAM_H_

#include <stream.h>
#include <vga.h>


class OutStream
        : public Stream
{
public:

    OutStream& KERNEL_CALL
    operator <<(const char*);

    OutStream& KERNEL_CALL
    operator <<(char);

    OutStream& KERNEL_CALL
    operator <<(Int);

    OutStream& KERNEL_CALL
    operator <<(UInt);

    OutStream& KERNEL_CALL
    operator <<(const void*);

    OutStream& KERNEL_CALL
    operator <<(IntegerBase);


private:



};



////// OutStream inlines //////

inline OutStream& KERNEL_CALL
OutStream::operator <<(const char* str)
{
    Vga::print(str);
    return *this;
}


inline OutStream& KERNEL_CALL
OutStream::operator <<(char c)
{
    Vga::print(c);
    return *this;
}


inline OutStream& KERNEL_CALL
OutStream::operator <<(Int num)
{
    Vga::print(intToStr(num, intBase()));
    return *this;
}


inline OutStream& KERNEL_CALL
OutStream::operator <<(UInt num)
{
    Vga::print(intToStr(num, intBase(), false));
    return *this;
}


inline OutStream& KERNEL_CALL
OutStream::operator <<(const void* ptr)
{
    Vga::print(intToStr(reinterpret_cast<UInt>(ptr), HEX, false));
    return *this;
}


inline OutStream& KERNEL_CALL
OutStream::operator <<(IntegerBase base)
{
    intBase(base);
    return *this;
}


#endif // _OUT_STREAM_H_
