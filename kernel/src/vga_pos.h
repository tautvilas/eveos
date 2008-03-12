#ifndef _VGA_POS_H_
#define _VGA_POS_H_

#include <global.h>
#include <vga.h>


class Vga::Pos
{
public:

    KERNEL_CALL
    Pos();

    KERNEL_CALL
    Pos(Index col, Index row);

    KERNEL_CALL
    Pos(const Pos& pos);

    const Index KERNEL_CALL
    col() const;

    const void KERNEL_CALL
    col(Index col);

    const Index KERNEL_CALL
    row() const;

    const void KERNEL_CALL
    row(Index row);

    bool KERNEL_CALL
    isValid() const;

    Pos& KERNEL_CALL
    operator ++();              // prefix

    Pos KERNEL_CALL
    operator ++(int);           // postfix

    Pos& KERNEL_CALL
    operator =(const Pos& pos);

    // :TODO: gx 7/28/2007: other operators


private:

    Size mCol;
    Size mRow;

    KERNEL_CALL
    Pos(Word index);

    //TSize KERNEL_CALL
    //ToIndex() const;

    KERNEL_CALL
    operator Index() const;

    friend class Vga;
    //friend class Vga::Caret;
};




////// Vga::Pos inlines //////

inline KERNEL_CALL
Vga::Pos::Pos()
        : mCol(0), mRow(0)
{
}


inline KERNEL_CALL
Vga::Pos::Pos(Size col, Size row)
        : mCol(col), mRow(row)
{
}


inline KERNEL_CALL
Vga::Pos::Pos(const Pos& pos)
        : mCol(pos.mCol), mRow(pos.mRow)
{
}


inline KERNEL_CALL
Vga::Pos::Pos(Word index)
        : mCol(index % COLS), mRow(index / COLS)
{
}


inline bool KERNEL_CALL
Vga::Pos::isValid() const
{
    return mCol < COLS && mRow < ROWS;
}


/*inline TSize KERNEL_CALL
Vga::TPos::ToIndex() const
{
    return mRow * COLS + mCol;
}*/


inline Vga::Pos& KERNEL_CALL
Vga::Pos::operator ++()
{
    ++mCol;
    if (mCol >= COLS)
    {
        mCol    = 0;
        ++mRow;
    }
    return *this;
}


inline Vga::Pos KERNEL_CALL
Vga::Pos::operator ++(int)
{
    Pos pos = *this;
    ++*this;
    return pos;
}


inline Vga::Pos& KERNEL_CALL
Vga::Pos::operator =(const Pos& pos)
{
    mCol    = pos.mCol;
    mRow    = pos.mRow;
    return *this;
}


inline KERNEL_CALL
Vga::Pos::operator Index() const
{
    return mRow * COLS + mCol;
}



#endif  // _VGA_H_
