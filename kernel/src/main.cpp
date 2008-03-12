#include <main.h>
#include <cpp_runtime.h>
//#include <idt.h>
//#include <mem.h>

#include <vga.h>
#include <out.h>


extern "C" void eve_main()
{
    CppRuntime::init();     // shouldn't we choose consitent naming
    //Idt::Install();         // for initialization?
    //Mem::init();

    // Vga tests
    if (true)
    {
        // screen corners
        Vga::put('\\', Vga::Pos(0, 0), Vga::BLACK, Vga::WHITE);
        Vga::put('/', Vga::Pos(79, 0), Vga::BLACK, Vga::WHITE);
        Vga::put('/', Vga::Pos(0, 24), Vga::BLACK, Vga::WHITE);
        Vga::put('\\', Vga::Pos(79, 24), Vga::BLACK, Vga::WHITE);

        // just a test of possioning and putting
        Vga::Pos   pos = Vga::Caret::pos();
        //Vga::Pos pos(0, 0);
        for (char* str = "Vga::Put()"; *str; ++str, ++pos)
            Vga::put(*str, pos, Vga::BLACK, Vga::WHITE);
        Vga::Caret::pos(pos);

        // real usage test
        Vga::print("\nVga::Print()");

        // scroll and color printing test
        //Vga::Caret::pos(Vga::Pos(0, 24));
        //Vga::print("Vga::Print()\n", Vga::BLACK, Vga::GREEN);

        /*/
        Vga::print(123, BIN);
        Vga::print(' ');
        Vga::print(123, OCT);
        Vga::print(' ');
        Vga::print(123, DEC);
        Vga::print(' ');
        Vga::print(123, HEX);
        Vga::print(' ');
        Vga::print(0xdeadbeef, HEX);
        //*/
    }

    // Out tests
    /*{
        Out::info() << "\nOut::info() test:\n"
                << 123 << ' '               // by default outputing in DEC
                << -123 << ' '

                << BIN << 123 << ' '        // we may use TIntegerBase as
                << OCT << 123 << ' '        // modifier to change integers
                << HEX << 123 << ' '        // base to output

                << 0xdeadbeef << ' '        // modifier once used has effect
                                            // on all output after it
                << DEC << 0xdeadbeef
                << "\n";
    }*/




    kernel_stop();
    //for (;;);
}
