#include <cpp_runtime.h>
#include <vga.h>

// resource: http://www.osdev.org/wiki/C_PlusPlus#Global_objects_2

extern "C"
{
    int
    __cxa_atexit(void (*func)(void *), void *arg, void *dso);

    void
    __cxa_finalize(void *d);

    void
    __cxa_pure_virtual();
};

void *__dso_handle; /*only the address of this symbol is taken by gcc*/


namespace
{
    const Size MAX_OBJECTS  = 32;

    struct Object
    {
            void (*mFunc)(void*);
            void *mArg;
            void *mDso;
    };

    Object  gObjects[MAX_OBJECTS];

    Size    gObjectCount    = 0;
};


void
CppRuntime::init()
{
    typedef void (*Ctor)();

    // from linker
    extern Ctor gCppCtorFirst;
    extern Ctor gCppCtorsEnd;

    const Ctor* ctor        = &gCppCtorFirst;
    const Ctor* ctorsEnd    = &gCppCtorsEnd;

    for (; ctor != ctorsEnd; ++ctor)
    {
        (*ctor)();
    }
}


void
CppRuntime::deinit()
{
    __cxa_finalize(0);
}


// Called automaticaly by GCC to register global/static objects destructors
int
__cxa_atexit(void (*func)(void *), void *arg, void *dso)
{
    if (gObjectCount >= MAX_OBJECTS)
    {
        // gx 8/18/2007: It's risky to use anything here but as long as
        // Vga::put() doesn't use any global/static objects its safe enough.
        Vga::put(
                "Maximum number of global/static objects destructors"
                    " exceeded!",
                Vga::Pos(0, 0),
                Vga::BLACK,
                Vga::RED
            );
        for (;;);

        return -1;
    }

    gObjects[gObjectCount].mFunc    = func;
    gObjects[gObjectCount].mArg     = arg;
    gObjects[gObjectCount].mDso     = dso;
    ++gObjectCount;
    return 0;
}


// This currently destroys all objects
void
__cxa_finalize(void *d)
{
    // Objects must be destroyed in opposite order they were constructed
    for (; gObjectCount != 0; --gObjectCount)
    {
        gObjects[gObjectCount].mFunc(gObjects[gObjectCount].mArg);
    }
}


// Called atomaticaly when call to pure virtual method detected.
// This should never happen as compiler woun't let anyone to call pure
// virutal methods. However it is possible using some bad hacks or in case
// of undefined behaviour of kernel.
void
__cxa_pure_virtual()
{
    Vga::put(
            "Call to pure virtual method detected!",
            Vga::Pos(0, 0),
            Vga::BLACK,
            Vga::RED
        );
    for (;;);
}
