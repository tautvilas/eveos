#include "main.h"

extern "C" void eve_main()
{
    {
        // just a test of possioning and putting
        Vga::Position   pos     = Vga::Caret::Pos();
        for (char* pStr = "Vga::Put()"; *pStr; ++pStr, ++pos)
            Vga::Put(*pStr, pos, Vga::BLACK, Vga::WHITE);
        Vga::Caret::Pos(pos);

        // real usage test
        Vga::Print("\nVga::Print()");

        // scroll and color printing test
        Vga::Caret::Pos(Vga::Position(0, 24));
        Vga::Print("Vga::Print()\n", Vga::BLACK, Vga::GREEN);
    }
    for (;;);
}

