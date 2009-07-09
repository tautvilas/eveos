#ifndef _OUT_H_
#define _OUT_H_

#include <out_stream.h>
#include <out_str.h>
#include <out_int.h>

class Out
{

public:

    static OutStream& 
    dbg();

    static OutStream& 
    info();

    static OutStream& 
    err();

    static OutStream& 
    warn();


private:

    static OutStream&    msDbg;
    static OutStream&    msInfo;
    static OutStream&    msErr;
    static OutStream&    msWarn;

    
    Out();

};



////// Out inlines //////

/*static*/ inline OutStream& 
Out::dbg()
{
    return msDbg;
}


/*static*/ inline OutStream& 
Out::info()
{
    return msInfo;
}


/*static*/ inline OutStream& 
Out::err()
{
    return msErr;
}


/*static*/ inline OutStream& 
Out::warn()
{
    return msWarn;
}


#endif // _OUT_H_
