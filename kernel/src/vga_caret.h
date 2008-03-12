#ifndef _VGA_CARET_H_
#define _VGA_CARET_H_

#include <vga.h>
#include <io_port.h>


/**
 *  Static class to maipulate VGA caret.
 */
class Vga::Caret
{
public:

    /**
     *  Returns ceret visibility.
     *
     *  @return true if caret visible, false otherwise.
     */
    static Bool KERNEL_CALL
    visible();

    /**
     *  Changes caret visibility.
     *
     *  @param  aVisible    true to male caret visible, false to hide caret.
     */
    static void KERNEL_CALL
    visible(Bool visible);

    /**
     *  Returns caret position.
     *
     *  @return Caret position.
     */
    static Pos KERNEL_CALL
    pos();

    /**
     *  Changes caret position.
     *
     *  @param  aPos    New caret position.
     */
    static void KERNEL_CALL
    pos(const Pos& pos);


private:

    class Reg;

    enum
    {
        HIDE_MASK     = 0x20,     // 00100000b
    };


    KERNEL_CALL
    Caret();
};


#include <vga_caret_reg.h>


#endif // _VGA_CARET_H_
