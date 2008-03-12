#include "stream.h"
#include "algorithms.h"

using namespace Generic;


/*static*/ char* KERNEL_CALL
Stream::intToStr(Int num, IntegerBase base, Bool isSigned)
{
    static const char   SYMBOLS[]   = {     // :DEPENDS ON: TIntegerBase
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            'a', 'b', 'c', 'd', 'e', 'f'
            };

    static const Size   BUFFER_SIZE =
            sizeof(unsigned int) * BITS_PER_BYTE
            + 1;            // for null-byte

    static char         sBuffer[BUFFER_SIZE];

    bool    negative;
    UInt    uint;

    if (isSigned && DEC == base)
    {
        negative    = num < 0;
        uint        = static_cast<UInt>(abs(num));
    }
    else
    {
        negative    = false;
        uint        = static_cast<UInt>(num);
    }

    char* str   = sBuffer + BUFFER_SIZE - 1;
    *str        = 0;
    for (; uint != 0; uint /= base)
        *--str = SYMBOLS[uint % base];

    str = intToStrPad(str, sBuffer + BUFFER_SIZE, base, SYMBOLS[0]);

    if (negative)
        *--str = '-';

    return str;
}


/*static*/ char* KERNEL_CALL
Stream::intToStrPad(char* str, char* end, IntegerBase base, char c)
{
    Size    div = 0;

    switch (base)
    {
        case HEX:
            div = sizeof(UInt) * 2;
        break;

        case BIN:
            div = BITS_PER_BYTE;
        break;

        default:
            return str;
        break;
    }

    Size    mod = (end - str - 1) % div;
    if (0 == mod)
        return str;

    Size len    = div - mod;
    str         -= len;
    fill(str, str + len, c);

    return str;
}
