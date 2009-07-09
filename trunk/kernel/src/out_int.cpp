#include <out_int.h>

#include <out_str.h>
#include <algorithms.h>


namespace {

    using namespace Generic;

    class IntStr
    {
    public:
    
        
        IntStr(const OutInt& i);
        
        const char* 
        str() const;
        
    private:
        static const char   SYMBOLS[];
        
        static const Size   BUFFER_SIZE = 
                sizeof(unsigned int) * BITS_PER_BYTE
                + 1             // for prefix
                + 1;            // for null-byte;
                
        const OutInt&       mInt;
        char                mBuffer[BUFFER_SIZE];
        char*               mStr;
        
        void 
        pad();
        
        void 
        prefix();
    };
    
}


OutInt::OutInt(UInt num)
        : base(DEC, this), prefix(false, this), pad(false, this), 
                isSigned(false, this), num(num, this)
{}


OutInt::OutInt(Int num)
        : base(DEC, this), prefix(false, this), pad(false, this),
                isSigned(true, this), num(num, this)
{}


OutInt::OutInt(const void* ptr)
        : base(HEX, this), prefix(false, this), pad(true, this),
                isSigned(false, this), num(reinterpret_cast<Int>(ptr), this)
{}


OutStream& 
operator <<(OutStream& out, const OutInt& i)
{
    return out << IntStr(i).str();
}


OutStream& 
operator <<(OutStream& out, Int i)
{
    return out << OutInt(i);
}


OutStream& 
operator <<(OutStream& out, UInt i)
{
    return out << OutInt(i);
}


OutStream& 
operator <<(OutStream& out, const void* ptr)
{
    return out << OutInt(ptr);
}


////// IntStr //////

    
/*static*/ const char   IntStr::SYMBOLS[]   = {     // :DEPENDS ON: IntegerBase
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F'
    };

    
IntStr::IntStr(const OutInt& i)
        : mInt(i), mStr(mBuffer + BUFFER_SIZE - 1)
{
    *mStr   = 0;

    Int num = mInt.num();
    if (0 == num)
    {
        *--mStr  = '0';
        pad();
        prefix();
    }
    else
    {
        bool        negative;
        UInt        uint;
        OutIntBase  base    = mInt.base();

        if (mInt.isSigned() && DEC == base)
        {
            negative    = num < 0;
            uint        = static_cast<UInt>(abs(num));
        }
        else
        {
            negative    = false;
            uint        = static_cast<UInt>(num);
        }

        for (; uint != 0; uint /= base)
            *--mStr = SYMBOLS[uint % base];

        pad();
        prefix();

        if (negative)
            *--mStr = '-';
    }    
}


void 
IntStr::pad()
{
    if (!mInt.pad())
        return;

    Size    div = 0;

    switch (mInt.base())
    {
        case HEX:
            div = sizeof(UInt) * 2;
        break;

        case BIN:
            div = BITS_PER_BYTE;
        break;

        default:
            return;
        break;
    }

    Size    mod = (mBuffer + BUFFER_SIZE - mStr - 1) % div;
    if (0 == mod)
        return;

    Size len    = div - mod;
    mStr        -= len;
    fill(mStr, mStr + len, SYMBOLS[0]);
}


void 
IntStr::prefix()
{
    if (!mInt.prefix())
        return;
        
    switch (mInt.base())
    {
        case BIN:
            *--mStr = 'b';
        break;
        
        case HEX:
            *--mStr = 'x';
        // no break
        
        case OCT:
            *--mStr = '0';
        break;
        
        default:
            // no prefix
            *--mStr = '?';
        break;
    }
}


const char* 
IntStr::str() const
{
    return mStr;
}


