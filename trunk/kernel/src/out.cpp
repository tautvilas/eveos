#include <out.h>
#include <vga_stream.h>

namespace {
    static VgaStream sDbg   = VgaStream(Vga::DARK_GRAY);
    static VgaStream sInfo  = VgaStream();
    static VgaStream sErr   = VgaStream(Vga::YELLOW, Vga::RED);
    static VgaStream sWarn  = VgaStream(Vga::YELLOW);
};

////// Out static members //////

OutStream& Out::msDbg   = sDbg;
OutStream& Out::msInfo  = sInfo;
OutStream& Out::msErr   = sErr;
OutStream& Out::msWarn  = sWarn;
