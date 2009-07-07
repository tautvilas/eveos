#include <debug.h>
#include <main.h>
#include <cpp_runtime.h>
#include <mem.h>
#include <new.h>
#include <isr.h>
#include <vga.h>
#include <out.h>

#include <critical.h>

extern "C" void kernel_idle();

extern "C" void eve_main()
{
    CppRuntime::init();
    Mem::init();
    Isr::init();

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
        for (const char* str = "Vga::Put()"; *str; ++str, ++pos)
            Vga::put(*str, pos, Vga::BLACK, Vga::WHITE);
        Vga::Caret::pos(pos);

        // real usage test
        Vga::print("\nVga::Print()\n");

        // scroll and color printing test
        //Vga::Caret::pos(Vga::Pos(0, 24));
        //Vga::print("Vga::Print()\n", Vga::BLACK, Vga::GREEN);
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

    // Kernel virtual memory tests
    /*{
        void* p = Mem::grow(0).value();
        DBG(p);

        Byte* p1 = static_cast<Byte*>(Mem::grow(4).value());
        DBG(p1);
        DBG(Mem::grow(2 * KILOBYTE).value());

        Byte* p2 = static_cast<Byte*>(Mem::grow(1).value());
        DBG(Mem::grow(0).value());
        DBG(p2);
        p2[0] = 'X';
        DBG(p2[0]);
        DBG(p1[4 + 2 * KILOBYTE]);
        DBG(p2[10]);    // accessing *formally* unallocated memory (ok)

        Byte* p3 = p2 + 4 * KILOBYTE;
        DBG(p3);
        //DBG(p3[0]);     // accessing *actually* unallocated memory (fault)
    }*/


    // operator new tests
    /*{
        DBG(Mem::used());
        Byte* p1 = new Byte('X');
        DBG(Mem::used());
        DBG(p1);
        DBG(*p1);
        Byte* p2 = new Byte[MEGABYTE / 2];
        DBG(Mem::used());
        DBG(p2);
        delete p1;
        delete [] p2;
    }*/

    /*
    CRITICAL
    {
        Out::info() << "critical " << Critical::depth() << "\n";
        CRITICAL
        {
            Out::info() << "critical " << Critical::depth() << "\n";
        }
        Out::info() << "critical " << Critical::depth() << "\n";
    }
    */

    // Interrupt tests
    /*
    {
        __asm__ ("int $0");
        __asm__ ("int $1");
        __asm__ ("int $2");
        __asm__ ("int $3");
        __asm__ ("int $31");
        __asm__ ("int $33");
    }
    */

    kernel_idle();
}
