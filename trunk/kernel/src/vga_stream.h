#ifndef _VGA_STREAM_
#define _VGA_STREAM_

#include <out_stream.h>
#include <vga.h>

class VgaStream
        : public OutStream
{
public:

    VgaStream();
    
    VgaStream(Vga::Color fg, Vga::Color bg = Vga::DEFAULT_BG);
    
    virtual void 
    write(char);
    
    virtual 
    ~VgaStream();
    
protected:

    Vga::Color  mFg;
    Vga::Color  mBg;
};


#endif  // _VGA_STREAM_
