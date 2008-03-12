#include <vga.h>
#include <algorithms.h>


////// Vga static constants //////

const Size  Vga::COLS       = 80;
const Size  Vga::ROWS       = 25;
const Size  Vga::TAB_WIDTH  = 8;
const char  Vga::BLANK_CHAR = ' ';

Vga::Chr* const Vga::MEM_BEGIN  = reinterpret_cast<Chr*>(0xB8000 + &gKernelBase);
//Vga::Chr* const Vga::MEM_BEGIN  = reinterpret_cast<Chr*>(0xB7F60 + &gKernelBase);
Vga::Chr* const Vga::MEM_END    = Vga::MEM_BEGIN + Vga::COLS * Vga::ROWS;


////// Vga static members //////

Vga::Color  Vga::msBgColor  = Vga::DEFAULT_BG;
Vga::Color  Vga::msFgColor  = Vga::DEFAULT_FG;



////// Vga methods //////

/*static*/ void KERNEL_CALL
Vga::print(char c, Color fg, Color bg)
{
    Pos pos = Caret::pos();

    switch (c)
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
                put(BLANK_CHAR, pos, fg, bg);
            }
        break;

        case '\b':
            if (pos.mCol > 0)
                --pos.mCol;
            put(BLANK_CHAR, pos, fg, bg);
        break;

        default:
            put(c, pos, fg, bg);
            ++pos;
        break;
    }

    if (ROWS == pos.mRow)
    {
        scrollUp();
        --pos.mRow;
    }

    Caret::pos(pos);
}


/*static*/ void KERNEL_CALL
Vga::print(const char* str, Color fg, Color bg)
{
    if (!str)
        return;

    for (; *str; ++str)
        print(*str, fg, bg);
}


/*static*/ void KERNEL_CALL
Vga::put(char c, const Pos& pos, Color fg, Color bg)
{
    if (!pos.isValid())
        return;

    MEM_BEGIN[pos]  = chr(c, fg, bg);
    //Chr* mem = reinterpret_cast<Chr*>(0xB8000 + &gKernelBase - 0);
    //mem[0] = chr('~', static_cast<Color>(5), static_cast<Color>(4));
}



/*static*/ void KERNEL_CALL
Vga::put(const char* str, const Pos& pos, Color fg, Color bg)
{
    if (!str)
        return;

    for (Pos p = pos; *str; ++str, ++p)
        put(*str, p, fg, bg);
}



/*static*/ void KERNEL_CALL
Vga::scrollUp(Size rows)
{
    using Generic::copy;
    using Generic::fill;
    Size scroll = rows * COLS;
    copy(MEM_BEGIN + scroll, MEM_END, MEM_BEGIN);
    fill(MEM_END - scroll, MEM_END, chr(BLANK_CHAR, msFgColor, msBgColor));
}
