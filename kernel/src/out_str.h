#ifndef _OUT_STR_H_
#define _OUT_STR_H_

#include <out_stream.h>
#include <types.h>
#include <property.h>


enum OutStrAlign
{
    LEFT,
    RIGHT,
    CENTER,
};


struct OutStr
{
    HAS_PROPERTIES(OutStr);   

    Property<Size>          width;
    Property<OutStrAlign>   align;
    Property<char>          fill;
    Property<const char*>   str; 

    
    OutStr(const char* str);
};


OutStream& 
operator <<(OutStream& out, const OutStr& str);


OutStream& 
operator <<(OutStream& out, const char* str);


OutStream& 
operator <<(OutStream& out, char);


OutStream& 
operator <<(OutStream& out, unsigned char);


#endif  // _OUT_STR_H_
