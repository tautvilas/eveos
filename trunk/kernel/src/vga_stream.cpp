#include <vga_stream.h>


VgaStream::VgaStream() 
        : OutStream(), mFg(Vga::DEFAULT_FG), mBg(Vga::DEFAULT_BG)
{}


VgaStream::VgaStream(Vga::Color fg, Vga::Color bg) 
        : OutStream(), mFg(fg), mBg(bg)
{}


VgaStream::~VgaStream()
{}


void 
VgaStream::write(char c)
{
    Vga::print(c, mFg, mBg);
}

