#include "vga.h"
#include "algorithms.h"


////// Vga static constants //////

const TSize  Vga::COLS       = 80;
const TSize  Vga::ROWS       = 25;
const TSize  Vga::TAB_WIDTH  = 8;
const char  Vga::BLANK_CHAR = ' ';

Vga::TChar* const    Vga::MEM_BEGIN  = reinterpret_cast<TChar*>(0xB8000 + &gKernelBase);
Vga::TChar* const    Vga::MEM_END    = Vga::MEM_BEGIN + Vga::COLS * Vga::ROWS;


////// Vga static members //////

Vga::TColor  Vga::msBgColor  = Vga::DEFAULT_BG;
Vga::TColor  Vga::msFgColor  = Vga::DEFAULT_FG;



////// Vga::Caret static members //////

TIoPort Vga::Caret::TReg::msAddrPort(Vga::Caret::TReg::ADDR_PORT);
TIoPort Vga::Caret::TReg::msDataPort(Vga::Caret::TReg::DATA_PORT);



////// Vga::Caret methods //////

/*static*/ bool KERNEL_CALL
Vga::Caret::Visible()
{
    return TReg(TReg::VISIBILITY).Read() & HIDE_MASK;
}


/*static*/ void KERNEL_CALL
Vga::Caret::Visible(bool aVisible)
{
    TReg reg(TReg::VISIBILITY);
    if (aVisible)
        reg.Write(reg.Read() & ~HIDE_MASK);
    else
        reg.Write(reg.Read() | HIDE_MASK);
}


/*static*/ Vga::TPos KERNEL_CALL
Vga::Caret::Pos()
{
    TWord   pos = 0 | TReg(TReg::POS_HI).Read();

    return pos << 8 | TReg(TReg::POS_LO).Read();
}


/*static*/ void KERNEL_CALL
Vga::Caret::Pos(const TPos& aPos)
{
    if (!aPos.IsValid())
        return;

    TReg(TReg::POS_HI).Write(static_cast<TByte>(aPos >> 8));
    TReg(TReg::POS_LO).Write(static_cast<TByte>(aPos));
}



////// Vga methods //////

/*static*/ void KERNEL_CALL
Vga::Print(char aChar, TColor aFgColor, TColor aBgColor)
{
    TPos pos    = Caret::Pos();

    switch (aChar)
    {
        case '\n':
        case '\r':
            pos.mRow++;
            pos.mCol    = 0;
        break;

        case '\t':
            for (TSize i = TAB_WIDTH - pos.mCol % TAB_WIDTH;
                    i != 0;
                    --i, ++pos
                )
            {
                Put(BLANK_CHAR, pos, aFgColor, aBgColor);
            }
        break;

        case '\b':
            if (pos.mCol > 0)
                --pos.mCol;
            Put(BLANK_CHAR, pos, aFgColor, aBgColor);
        break;

        default:
            Put(aChar, pos, aFgColor, aBgColor);
            ++pos;
        break;
    }

    if (ROWS == pos.mRow)
    {
        ScrollUp();
        --pos.mRow;
    }

    Caret::Pos(pos);
}


/*static*/ void KERNEL_CALL
Vga::Print(const char* apStr, TColor aFgColor, TColor aBgColor)
{
    if (!apStr)
        return;

    for (; *apStr; ++apStr)
        Print(*apStr, aFgColor, aBgColor);
}


/*static*/ void KERNEL_CALL
Vga::Put(char aChar, const TPos& aPos, TColor aFgColor, TColor aBgColor)
{
    if (!aPos.IsValid())
        return;

    MEM_BEGIN[aPos] = Char(aChar, aFgColor, aBgColor);
}



/*static*/ void KERNEL_CALL
Vga::Put(const char* apStr, const TPos& aPos, TColor aFgColor,
        TColor aBgColor)
{
    if (!apStr)
        return;

    for (TPos pos = aPos; *apStr; ++apStr, ++pos)
        Put(*apStr, pos, aFgColor, aBgColor);
}


/*static*/ void KERNEL_CALL
Vga::Put(int aInt, TIntegerBase aBase, const TPos& aPos, TColor aFgColor,
        TColor aBgColor)
{
    Put(IntToStr(aInt, aBase), aPos, aFgColor, aBgColor);
}


/*static*/ void KERNEL_CALL
Vga::Print(int aInt, TIntegerBase aBase, TColor aFgColor, TColor aBgColor)
{
    Print(IntToStr(aInt, aBase), aFgColor, aBgColor);
}


/*static*/ void KERNEL_CALL
Vga::ScrollUp(TSize aRows)
{
    using Generic::Copy;
    using Generic::Fill;
    TSize scroll = aRows * COLS;
    Copy(MEM_BEGIN + scroll, MEM_END, MEM_BEGIN);
    Fill(MEM_END - scroll, MEM_END, Char(BLANK_CHAR, msFgColor, msBgColor));
}


/*static*/ char* KERNEL_CALL
Vga::IntToStr(int aInt, TIntegerBase aBase)
{
    switch (aBase)
    {
        case DEC:
            return UIntToStr(Generic::Abs(aInt), aBase, aInt < 0);

        default:
            return UIntToStr(static_cast<unsigned int>(aInt), aBase, false);
    }
}


/*static*/ char* KERNEL_CALL
Vga::UIntToStr(unsigned int aInt, TIntegerBase aBase, bool aNegative)
{
    static const char   SYMBOLS[]   = { '0', '1', '2', '3', '4', '5', '6',
             '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    static const TSize  PREFIX_SIZE = 3;

    static const TSize  BUFFER_SIZE = sizeof(unsigned int) * BITS_PER_BYTE
            + 1             // for sign
            + PREFIX_SIZE
            + 1;            // for null-byte

    static char         spBuffer[BUFFER_SIZE];

    char* pStr  = spBuffer + BUFFER_SIZE - 1;
    *pStr       = 0;
    for (; aInt != 0; aInt /= aBase)
        *--pStr = SYMBOLS[aInt % aBase];


    static const TSize          BASE_COUNT          = 4;
    static const TIntegerBase   BASES[BASE_COUNT]   = { BIN, OCT, DEC, HEX };
    TSize base_idx  = Generic::Find(BASES, BASES + BASE_COUNT, aBase) - BASES;

    // right padding (should be seperate method)
    {
        static const TSize  PAD_DIVS[]   = {
                BITS_PER_BYTE,              // bin
                0,                          // oct
                0,                          // dec
                sizeof(unsigned int) * 2,   // hex
                };
        TSize div   = PAD_DIVS[base_idx];
        if (div)
        {
            TSize mod   = (spBuffer + BUFFER_SIZE - pStr - 1) % div;
            if (mod)
            {
                TSize len   = div - mod;
                pStr        -= len;
                Generic::Fill(pStr, pStr + len, SYMBOLS[0]);
            }
        }
    }

    // adding prefix (should be seperate method)
    {
        typedef char TPrefix[PREFIX_SIZE];
        static const TPrefix    PREFIXES[BASE_COUNT]    = {
                "",         // bin
                "0",        // oct
                "",         // dec
                "0x",       // hex
                };

        const char* PREFIX  = PREFIXES[base_idx];
        TSize prefix_len    = Generic::Find(PREFIX, PREFIX + PREFIX_SIZE, '\0')
                - PREFIX;
        pStr                -= prefix_len;
        Generic::Copy(PREFIX, PREFIX + prefix_len, pStr);
    }

    if (aNegative)
        *--pStr = '-';

    return pStr;
}

