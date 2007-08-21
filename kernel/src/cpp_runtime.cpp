#include "cpp_runtime.h"
#include "vga.h"

// resource: http://www.osdev.org/wiki/C_PlusPlus#Global_objects_2

extern "C"
{
    int
    __cxa_atexit(void (*apFunc)(void *), void *apArg, void *apDso);

    void
    __cxa_finalize(void *d);

    void
    __cxa_pure_virtual();
};

void *__dso_handle; /*only the address of this symbol is taken by gcc*/


namespace
{
    const TSize MAX_OBJECTS  = 32;

    struct TObject
    {
            void (*mpFunc)(void*);
            void *mpArg;
            void *mpDso;
    };

    TObject gspObjects[MAX_OBJECTS];

    TSize   gObjectCount    = 0;
};


void
CppRuntime::Init()
{
    typedef void (*TCtor)();

    // from linker
    extern TCtor gCppCtorFirst;
    extern TCtor gCppCtorsEnd;

    const TCtor* pCtor      = &gCppCtorFirst;
    const TCtor* pCtorsEnd  = &gCppCtorsEnd;

    for (; pCtor != pCtorsEnd; ++pCtor)
    {
        (*pCtor)();
    }
}


void
CppRuntime::Deinit()
{
    __cxa_finalize(0);
}


// Called automaticaly by GCC to register global/static objects destructors
int
__cxa_atexit(void (*apFunc)(void *), void *apArg, void *apDso)
{
    if (gObjectCount >= MAX_OBJECTS)
    {
        // gx 8/18/2007: It's risky to use anything here but as long as
        // Vga::Put() doesn't use any global/static objects its safe enough.
        Vga::Put(
            "Maximum number of global/static objects destructors exceeded!",
            Vga::TPos(0, 0),
            Vga::BLACK,
            Vga::RED
            );
        for (;;);

        return -1;
    }

    gspObjects[gObjectCount].mpFunc = apFunc;
    gspObjects[gObjectCount].mpArg  = apArg;
    gspObjects[gObjectCount].mpDso  = apDso;
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
        gspObjects[gObjectCount].mpFunc(gspObjects[gObjectCount].mpArg);
    }
}


// Called atomaticaly when call to pure virtual method detected.
// This should never happen as compiler woun't let anyone to call pure
// virutal methods. However it is possible using some bad hacks or in case
// of undefined behaviour of kernel.
void
__cxa_pure_virtual()
{
    Vga::Put(
            "Call to pure virtual method detected!",
            Vga::TPos(0, 0),
            Vga::BLACK,
            Vga::RED
            );
    for (;;);
}

