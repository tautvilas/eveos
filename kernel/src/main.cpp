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

    // DUMP test
    //*
    {
        char* str = "hello world!";
        DUMP(str - 53, 111);
    }//*/

    // new OutStream tests
    /*
    {
        Out::dbg()  << "hi\n";
        Out::info() << "hi\n";
        Out::err()  << "hi\n";
        Out::warn() << "hi\n";
        Out::dbg()  << 'a' << "\nb";
        Out::dbg() 
                << '\n' << OutStr("hi").align(RIGHT) // NOP -- no width
                << '\n' << OutStr("hi").width(5).fill('.').align(LEFT)
                << '\n' << OutStr("hi").width(5).fill('-').align(CENTER)
                << '\n' << OutStr("hi!").width(5).fill('-').align(CENTER)
                << '\n';
                
        DBG("hi");
        char s[] = "hi again";
        DBG(s);
        DBG('a');
        char c = 'b';
        DBG(c);
        DBG(123);
        UInt u = 0xdeadbeef;
        DBG(u);
        Int i = -123;
        DBG(i);
        typedef void* vp;
        vp p = vp(5);
        DBG(p);
        void* p2 = p;
        p = p2;
        DBG(p);
    }//*/
    
    // Maybe tests
    /*
    {
        Maybe<char> m;
        ASSERT(!m);
        if (m)
            ASSERT(false);
        ASSERT(false == m);
        m = 0;
        ASSERT(m);
        ASSERT(false == !m);
        if (!m)
            ASSERT(false);
        
        //void* p = m;
        //int i = m;
    }//*/
    
    // Vga tests
    /*if (true)
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
    }*/

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
        Out::info() << "Zeros: "
                << BIN << 0 << ' ' 
                << OCT << 0 << ' '
                << HEX << 0 << ' '
                << DEC << 0 << ' '
                << "\n";
                
        DBG(0);
    }//*/

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
    }//*/


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

    // CRITICAL tests
    /*
    const int wait = 200;
    for (int i = wait; --i;) DBG(i);
    CRITICAL
    {
        Out::info() << "critical " << Critical::depth() << "\n";
        for (int crit = wait; --crit;) DBG(crit);
        CRITICAL
        {
            Out::info() << "critical " << Critical::depth() << "\n";
            for (int critical = wait; --critical;) DBG(critical);
        }
        Out::info() << "critical " << Critical::depth() << "\n";
        for (int crit = wait*3; --crit;) DBG(crit);
    }
    for (int i = wait; --i;) DBG(i);
    //*/

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
