#ifndef _VGA_CARET_REG_H_
#define _VGA_CARET_REG_H_

#include <vga_caret.h>
#include <io_port.h>


/**
 *  Class representing VGA caret register.
 */
class Vga::Caret::Reg
{
public:

    enum RegIndex
    {
        POS_HI      = 0x0E,
        POS_LO      = 0x0F,
        VISIBILITY  = 0x0A,     // actually this is "caret start
                                // register" but as we use it only
                                // for visibility lets name it this
                                // way
    };


    /**
     *  Constructor.
     *
     *  @param  regIndex    VGA caret register index.
     */
    KERNEL_CALL
    Reg(RegIndex regIndex);

    /**
     *  Reads and returns Byte of data from register.
     *
     *  @return Byte of data from register.
     */
    Byte KERNEL_CALL
    read();

    /**
     *  Writes Byte of data to register.
     *
     *  @param  data    Data to write to register.
     */
    void KERNEL_CALL
    write(Byte data);

private:

    enum
    {
        ADDR_PORT   = 0x3d4,
        DATA_PORT   = 0x3d5,
    };

    static IoPort   msAddrPort;
    static IoPort   msDataPort;

    RegIndex        mRegIndex;
};




////// Vga::Caret::Reg inlines //////

inline KERNEL_CALL
Vga::Caret::Reg::Reg(RegIndex regIndex)
        : mRegIndex(regIndex)
{
}


inline void KERNEL_CALL
Vga::Caret::Reg::write(Byte data)
{
    msAddrPort.writeByte(mRegIndex);
    msDataPort.writeByte(data);
}


inline Byte KERNEL_CALL
Vga::Caret::Reg::read()
{
    msAddrPort.writeByte(mRegIndex);
    return msDataPort.readByte();
}


#endif // _VGA_CARET_REG_H_
