#ifndef _STREAM_H_
#define _STREAM_H_

#include <global.h>


class Stream
{
public:

    void KERNEL_CALL
    intBase(IntegerBase base);

    IntegerBase KERNEL_CALL
    intBase();



protected:

    KERNEL_CALL
    Stream();

    static char* KERNEL_CALL
    intToStr(Int num, IntegerBase base, Bool isSigned = true);

private:

    IntegerBase     mIntBase;


    static char* KERNEL_CALL
    intToStrPad(char* str, char* end, IntegerBase base, char c);
};



////// Stream inlines //////

inline KERNEL_CALL
Stream::Stream()
        : mIntBase(DEC)
{
}


inline void KERNEL_CALL
Stream::intBase(IntegerBase base)
{
    mIntBase    = base;
}

inline IntegerBase KERNEL_CALL
Stream::intBase()
{
    return mIntBase;
}


#endif // _STREAM_H_
