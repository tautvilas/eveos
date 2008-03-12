#include <vga_caret.h>



////// Vga::Caret methods //////

/*static*/ bool KERNEL_CALL
Vga::Caret::visible()
{
    return Reg(Reg::VISIBILITY).read() & HIDE_MASK;
}


/*static*/ void KERNEL_CALL
Vga::Caret::visible(Bool visible)
{
    Reg reg(Reg::VISIBILITY);
    if (visible)
        reg.write(reg.read() & ~HIDE_MASK);
    else
        reg.write(reg.read() | HIDE_MASK);
}


/*static*/ Vga::Pos KERNEL_CALL
Vga::Caret::pos()
{
    Word    pos = 0 | Reg(Reg::POS_HI).read();

    return pos << 8 | Reg(Reg::POS_LO).read();
}


/*static*/ void KERNEL_CALL
Vga::Caret::pos(const Pos& pos)
{
    if (!pos.isValid())
        return;

    Reg(Reg::POS_HI).write(static_cast<Byte>(pos >> 8));
    Reg(Reg::POS_LO).write(static_cast<Byte>(pos));
}
