#ifndef _OUT_H_
#define _OUT_H_

#include "out_stream.h"

class Out
{

public:

    static TOutStream& KERNEL_CALL
    Dbg();

    static TOutStream& KERNEL_CALL
    Info();

    static TOutStream& KERNEL_CALL
    Err();

    static TOutStream& KERNEL_CALL
    Warn();


private:

    static TOutStream   mInfo;

    KERNEL_CALL
    Out();

};



////// Out inlines //////

/*static*/ inline TOutStream& KERNEL_CALL
Out::Dbg()
{
    return mInfo;
}


/*static*/ inline TOutStream& KERNEL_CALL
Out::Info()
{
    return mInfo;
}


/*static*/ inline TOutStream& KERNEL_CALL
Out::Err()
{
    return mInfo;
}


/*static*/ inline TOutStream& KERNEL_CALL
Out::Warn()
{
    return mInfo;
}


#endif // _OUT_H_

