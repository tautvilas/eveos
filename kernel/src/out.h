#ifndef _OUT_H_
#define _OUT_H_

#include <out_stream.h>

class Out
{

public:

    static OutStream& KERNEL_CALL
    dbg();

    static OutStream& KERNEL_CALL
    info();

    static OutStream& KERNEL_CALL
    err();

    static OutStream& KERNEL_CALL
    warn();


private:

    static OutStream    msInfo;

    KERNEL_CALL
    Out();

};



////// Out inlines //////

/*static*/ inline OutStream& KERNEL_CALL
Out::dbg()
{
    return msInfo;
}


/*static*/ inline OutStream& KERNEL_CALL
Out::info()
{
    return msInfo;
}


/*static*/ inline OutStream& KERNEL_CALL
Out::err()
{
    return msInfo;
}


/*static*/ inline OutStream& KERNEL_CALL
Out::warn()
{
    return msInfo;
}


#endif // _OUT_H_
