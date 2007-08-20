#include "main.h"

#include "cpp_runtime.h"

extern "C" void eve_main()
{
    CppRuntime::Init();

    {
        // just a test of possioning and putting
        Vga::TPos   pos = Vga::Caret::Pos();
        for (char* pStr = "Vga::Put()"; *pStr; ++pStr, ++pos)
            Vga::Put(*pStr, pos, Vga::BLACK, Vga::WHITE);
        Vga::Caret::Pos(pos);

        // real usage test
        Vga::Print("\nVga::Print()");

        // scroll and color printing test
        Vga::Caret::Pos(Vga::TPos(0, 24));
        Vga::Print("Vga::Print()\n", Vga::BLACK, Vga::GREEN);

        Vga::Print(123, BIN);
        Vga::Print(' ');
        Vga::Print(123, OCT);
        Vga::Print(' ');
        Vga::Print(123, DEC);
        Vga::Print(' ');
        Vga::Print(123, HEX);
        Vga::Print(' ');
        Vga::Print(0xdeadbeef, HEX);
    }



    for (;;);
}

