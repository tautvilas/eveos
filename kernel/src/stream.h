#ifndef _STREAM_H_
#define _STREAM_H_

#include "global.h"


class TStream
{
public:

    void KERNEL_CALL
    IntBase(TIntegerBase aBase);

    TIntegerBase KERNEL_CALL
    IntBase();



protected:

    KERNEL_CALL
    TStream();

    static char* KERNEL_CALL
    IntToStr(TInt aInt, TIntegerBase aBase);

private:

    TIntegerBase    mIntBase;


    static char* KERNEL_CALL
    IntToStrPad(char* apStr, char* apEnd, TIntegerBase aBase, char aChar);
};



////// TStream inlines //////

inline KERNEL_CALL
TStream::TStream()
        : mIntBase(DEC)
{
}


inline void KERNEL_CALL
TStream::IntBase(TIntegerBase aBase)
{
    mIntBase    = aBase;
}

inline TIntegerBase KERNEL_CALL
TStream::IntBase()
{
    return mIntBase;
}


#endif // _STREAM_H_

