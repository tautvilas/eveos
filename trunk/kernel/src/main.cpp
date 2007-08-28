#include "main.h"
#include "cpp_runtime.h"
#include "idt.h"

#include "vga.h"
#include "out.h"


extern "C" void eve_main()
{
    CppRuntime::Init();     // shouldn't we choose consitent naming
    Idt::Install();         // for initialization?

    // Vga tests
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

        /*
        Vga::Print(123, BIN);
        Vga::Print(' ');
        Vga::Print(123, OCT);
        Vga::Print(' ');
        Vga::Print(123, DEC);
        Vga::Print(' ');
        Vga::Print(123, HEX);
        Vga::Print(' ');
        Vga::Print(0xdeadbeef, HEX);
        */
    }

    // Out tests
    {
        Out::Info() << "Out::Info() test:\n"
                << 123 << ' '               // by default outputing in DEC
                << -123 << ' '

                << BIN << 123 << ' '        // we may use TIntegerBase as
                << OCT << 123 << ' '        // modifier to change integers
                << HEX << 123 << ' '        // base to output

                << 0xdeadbeef << ' '        // modifier once used has effect
                                            // on all output after it
                << DEC << 0xdeadbeef
                << "\n";
    }


    //for (;;);
    kernel_stop();
}

