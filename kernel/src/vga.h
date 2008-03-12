#ifndef _VGA_H_
#define _VGA_H_

#include <global.h>


class Vga
{
public:

    enum Color
    {
        BLACK                   = 0,
        BLUE                    = 1,
        GREEN                   = 2,
        CYAN                    = 3,
        RED                     = 4,
        MAGENTA                 = 5,
        BROWN                   = 6,
        LIGHT_GRAY              = 7,
        DARK_GRAY               = 8,
        LIGHT_BLUE              = 9,
        LIGHT_GREEN             = 10,
        LIGHT_CYAN              = 11,
        LIGHT_RED               = 12,
        LIGHT_MAGENTA           = 13,
        YELLOW                  = 14,
        WHITE                   = 15,

        DEFAULT_FG              = LIGHT_GRAY,
        DEFAULT_BG              = BLACK,
    };


    class Pos;
    class Caret;


    static void KERNEL_CALL
    put(char c, const Pos& pos, Color fg, Color bg);

    static void KERNEL_CALL
    put(char aChar, const Pos& aPos);


    static void KERNEL_CALL
    put(const char* str, const Pos& pos, Color fg, Color bg);

    static void KERNEL_CALL
    put(const char* str, const Pos& pos);



    static void KERNEL_CALL
    print(char c, Color fg, Color bg);

    static void KERNEL_CALL
    print(const char c);


    static void KERNEL_CALL
    print(const char* str, Color fg, Color bg);

    static void KERNEL_CALL
    print(const char* str);



    static const Color KERNEL_CALL
    foreground();

    static const void KERNEL_CALL
    foreground(Color fg);


    static const Color KERNEL_CALL
    background();

    static const void KERNEL_CALL
    background(Color bg);


private:

    typedef Word        Chr;

    static Chr* const   MEM_BEGIN;
    static Chr* const   MEM_END;

    static const Size   COLS;
    static const Size   ROWS;
    static const Size   TAB_WIDTH;
    static const char   BLANK_CHAR;


    static Color        msBgColor;
    static Color        msFgColor;

    KERNEL_CALL
    Vga();

    static Chr KERNEL_CALL
    chr(char c, Color fg, Color bg);

    static void KERNEL_CALL
    scrollUp(Size aLines = 1);

    static char* KERNEL_CALL
    intToStr(int i, IntegerBase base);

    static char* KERNEL_CALL
    uintToStr(unsigned int ui, IntegerBase base, Bool negative);

    //friend Pos;
};



#include <vga_pos.h>
#include <vga_caret.h>




////// Vga inlines //////


/*static*/ inline void KERNEL_CALL
Vga::put(char c, const Pos& pos)
{
    put(c, pos, msFgColor, msBgColor);
}


/*static*/ inline void KERNEL_CALL
Vga::put(const char* str, const Pos& pos)
{
    put(str, pos, msFgColor, msBgColor);
}


/*static*/ inline void KERNEL_CALL
Vga::print(const char c)
{
    print(c, msFgColor, msBgColor);
}


/*static*/ inline void KERNEL_CALL
Vga::print(const char* str)
{
    print(str, msFgColor, msBgColor);
}


/*static*/ inline const Vga::Color KERNEL_CALL
Vga::foreground()
{
    return msFgColor;
}


/*static*/ inline const void KERNEL_CALL
Vga::foreground(Color color)
{
    msFgColor   = color;
}


/*static*/ inline const Vga::Color KERNEL_CALL
Vga::background()
{
    return msBgColor;
}


/*static*/ inline const void KERNEL_CALL
Vga::background(Color color)
{
    msBgColor   = color;
}


/*static*/ inline Vga::Chr KERNEL_CALL
Vga::chr(char c, Color fg, Color bg)
{
    return static_cast<Chr>(c) | fg << 8 | bg << 12;
}


#endif  // _VGA_H_
