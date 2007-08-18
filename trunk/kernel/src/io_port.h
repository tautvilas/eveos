#ifndef _IO_PORT_H_
#define _IO_PORT_H_

#include "global.h"

/**
 *  Class for I/O ports manipulation.
 */
class IoPort
{

    // gx 8/18/2007: tried overloading and template approaches for Write
    // method with different size data like Byte, Word, etc. but ended up
    // with named method solution because we need strict control over values
    // we write to I/O port. With overloaded/template method we'd need
    // explicitly cast values to use right method implementation. With named
    // methods casting is performed implicitly by compiler and we always pass
    // right size values to the I/O port.
    // We actually could have template implementation of Read method as we
    // don't loose any control, but the actual call would look a bit ulgy:
    //       port.Read<Byte>()
    // For interface consitency and simplicity I think named methods is
    // better approach.
    //
    // Maybe it would be nice to have streaming operators on this class.
    // However we'd have the same problem with writing as dusscused above.

public:

    KERNEL_CALL
    IoPort(Word aPort);

    void KERNEL_CALL
    WriteByte(Byte aData);

    Byte KERNEL_CALL
    ReadByte();

    // :TODO: gx 8/18/2007: mothods for other data types like when needed.


private:
    Word    mPort;

};



////// IoPort inlines //////

inline KERNEL_CALL
IoPort::IoPort(Word aPort)
        : mPort(aPort)
{
}


inline void KERNEL_CALL
IoPort::WriteByte(Byte aData)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN"(mPort), "a"(aData));
}


inline Byte KERNEL_CALL
IoPort::ReadByte()
{
    Byte data;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (data) : "dN" (mPort));
    return data;
}


#endif // _IO_PORT_H_
