#ifndef _OUT_INT_
#define _OUT_INT_

#include <out_stream.h>
#include <types.h>
#include <property.h>


enum OutIntBase
{
    BIN     = 2,
    OCT     = 8,
    DEC     = 10,
    HEX     = 16,
};


struct OutInt
{
    HAS_PROPERTIES(OutInt);
    
    Property<OutIntBase>    base;
    Property<bool>          prefix;
    Property<bool>          pad;
    Property<bool>          isSigned;
    Property<Int>           num;
    
    
    OutInt(UInt num);
    
    
    OutInt(Int num);
    
    
    OutInt(const void* ptr);
};


OutStream& 
operator <<(OutStream& out, const OutInt& i);


OutStream& 
operator <<(OutStream& out, Int i);


OutStream& 
operator <<(OutStream& out, UInt i);


OutStream& 
operator <<(OutStream& out, const void* ptr);


#endif  // _OUT_INT_
