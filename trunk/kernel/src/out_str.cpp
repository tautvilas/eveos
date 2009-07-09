#include <out_str.h>
#include <algorithms.h>


namespace {

    Size 
    length(const char* str)
    {
        const char* end = Generic::find(str, str - 1, 0);
        return (end > str) ? end - str : 0;
    }
    
    
    struct Padding
    {
        Size    left;
        Size    right;
        
        Padding(OutStr str, Size len);
    };
}



OutStr::OutStr(const char* str)
        : width(0, this), align(LEFT, this), fill(' ', this), str(str, this)
{}


OutStream& 
operator <<(OutStream& out, const char* str)
{
    for (; *str; out.write(*str++));
    return out;
}


OutStream& 
operator <<(OutStream& out, char c)
{
    out.write(c);
    return out;
}


OutStream& 
operator <<(OutStream& out, unsigned char c)
{
    out.write(c);
    return out;
}


OutStream& 
operator <<(OutStream& out, const OutStr& str)
{
    Size len = length(str.str());
    
    if (0 == len)
        return out;
        
    Padding pad(str, len);
    for (; pad.left--; out << str.fill());
    out << str.str();
    for (; pad.right--; out << str.fill());

    return out;
}


Padding::Padding(OutStr str, Size len)
        : left(0), right(0)
{
    if (len >= str.width())
        return;
        
    switch (str.align())
    {
        case LEFT:
            right   = str.width() - len;
        break;
        
        case RIGHT:
            left    = str.width() - len;
        break;
        
        case CENTER:
            left    = (str.width() - len) / 2;
            right   = Generic::divUp(str.width() - len,  2);
        break;
        
        default:
            // ignore
        break;
    }
}

