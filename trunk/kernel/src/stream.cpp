#include "stream.h"
#include "algorithms.h"

using namespace Generic;


/*static*/ char* KERNEL_CALL
TStream::IntToStr(TInt aInt, TIntegerBase aBase, bool aSigned)
{
    static const char   SYMBOLS[]   = {     // :DEPENDS ON: TIntegerBase
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            'a', 'b', 'c', 'd', 'e', 'f'
            };

    static const TSize  BUFFER_SIZE =
            sizeof(unsigned int) * BITS_PER_BYTE
            + 1;            // for null-byte

    static char         spBuffer[BUFFER_SIZE];

    bool    negative;
    TUInt   uint;

    if (aSigned && DEC == aBase)
    {
        negative    = aInt < 0;
        uint        = static_cast<TUInt>(Abs(aInt));
    }
    else
    {
        negative    = false;
        uint        = static_cast<TUInt>(aInt);
    }

    char* pStr  = spBuffer + BUFFER_SIZE - 1;
    *pStr       = 0;
    for (; uint != 0; uint /= aBase)
        *--pStr = SYMBOLS[uint % aBase];

    pStr    = IntToStrPad(pStr, spBuffer + BUFFER_SIZE, aBase, SYMBOLS[0]);

    if (negative)
        *--pStr = '-';

    return pStr;
}


/*static*/ char* KERNEL_CALL
TStream::IntToStrPad(char* apStr, char* apEnd, TIntegerBase aBase, char aChar)
{
    TSize   div = 0;

    switch (aBase)
    {
        case HEX:
            div = sizeof(unsigned int) * 2;
        break;

        case BIN:
            div = BITS_PER_BYTE;
        break;

        default:
            return apStr;
        break;
    }

    TSize mod   = (apEnd - apStr - 1) % div;
    if (0 == mod)
        return apStr;

    TSize len   = div - mod;
    apStr       -= len;
    Fill(apStr, apStr + len, aChar);

    return apStr;
}

