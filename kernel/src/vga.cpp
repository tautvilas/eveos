#include "vga.h"


////// Vga static constants //////

Word* const Vga::ADDRESS    = reinterpret_cast<Word*>(0xB8000 + &gKernelBase);
const Size  Vga::COLS       = 80;
const Size  Vga::ROWS       = 25;
const Size  Vga::TAB_WIDTH  = 8;
const char  Vga::BLANK_CHAR = ' ';



////// Vga static members //////

Vga::Color  Vga::msBgColor  = Vga::DEFAULT_BG;
Vga::Color  Vga::msFgColor  = Vga::DEFAULT_FG;



////// Vga::Caret static members //////

//IoPort Vga::Caret::Reg::msAddrPort(0x3d4);
//IoPort Vga::Caret::Reg::msDataPort(0x3d5);



////// Vga::Caret methods //////

/*static*/ bool KERNEL_CALL
Vga::Caret::Visible()
{
    return Reg(Reg::VISIBILITY).Read() & HIDE_MASK;
}


/*static*/ void KERNEL_CALL
Vga::Caret::Visible(bool aVisible)
{
    Reg reg(Reg::VISIBILITY);
    if (aVisible)
        reg.Write(reg.Read() & ~HIDE_MASK);
    else
        reg.Write(reg.Read() | HIDE_MASK);
}


/*static*/ Vga::Position KERNEL_CALL
Vga::Caret::Pos()
{
    Word    pos = 0 | Reg(Reg::POS_HI).Read();

    return pos << 8 | Reg(Reg::POS_LO).Read();
}


/*static*/ void KERNEL_CALL
Vga::Caret::Pos(const Position& aPos)
{
    if (!aPos.IsValid())
        return;

    Reg(Reg::POS_HI).Write(static_cast<Byte>(aPos >> 8));
    Reg(Reg::POS_LO).Write(static_cast<Byte>(aPos));
}



////// Vga methods //////

/*static*/ void KERNEL_CALL
Vga::Print(char aChar, Color aFgColor, Color aBgColor)
{
    Position pos    = Caret::Pos();

    switch (aChar)
    {
        case '\n':
        case '\r':
            pos.mRow++;
            pos.mCol    = 0;
        break;

        case '\t':
            for (Size i = TAB_WIDTH - pos.mCol % TAB_WIDTH;
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
Vga::Print(const char* apStr, Color aFgColor, Color aBgColor)
{
    if (!apStr)
        return;

    for (; *apStr; ++apStr)
        Print(*apStr, aFgColor, aBgColor);
}


/*static*/ void KERNEL_CALL
Vga::Put(char aChar, const Position& aPos, Color aFgColor, Color aBgColor)
{
    if (!aPos.IsValid())
        return;

    ADDRESS[aPos] = Char(aChar, aFgColor, aBgColor);
}



/*static*/ void KERNEL_CALL
Vga::Put(const char* apStr, const Position& aPos, Color aFgColor,
        Color aBgColor)
{
    if (!apStr)
        return;

    for (Position pos = aPos; *apStr; ++apStr, ++pos)
        Put(*apStr, pos, aFgColor, aBgColor);
}


/*static*/ void KERNEL_CALL
Vga::ScrollUp(Size aRows)
{
    Size size   = (ROWS - aRows) * COLS;
    Mem::Copy(ADDRESS, ADDRESS + COLS, size);
    Mem::Set(ADDRESS + size, COLS, Char(BLANK_CHAR, msFgColor, msBgColor));
}

