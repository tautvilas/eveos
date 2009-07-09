#ifndef _VGA_POS_H_
#define _VGA_POS_H_

#include <global.h>
#include <vga.h>


class Vga::Pos
{
public:

    
    Pos();

    
    Pos(Index col, Index row);

    
    Pos(const Pos& pos);

    const Index 
    col() const;

    const void 
    col(Index col);

    const Index 
    row() const;

    const void 
    row(Index row);

    bool 
    isValid() const;

    Pos& 
    operator ++();              // prefix

    Pos 
    operator ++(int);           // postfix

    Pos& 
    operator =(const Pos& pos);

    // :TODO: gx 7/28/2007: other operators


private:

    Size mCol;
    Size mRow;

    
    Pos(Word index);

    //TSize 
    //ToIndex() const;

    
    operator Index() const;

    friend class Vga;
    //friend class Vga::Caret;
};




////// Vga::Pos inlines //////

inline 
Vga::Pos::Pos()
        : mCol(0), mRow(0)
{
}


inline 
Vga::Pos::Pos(Size col, Size row)
        : mCol(col), mRow(row)
{
}


inline 
Vga::Pos::Pos(const Pos& pos)
        : mCol(pos.mCol), mRow(pos.mRow)
{
}


inline 
Vga::Pos::Pos(Word index)
        : mCol(index % COLS), mRow(index / COLS)
{
}


inline bool 
Vga::Pos::isValid() const
{
    return mCol < COLS && mRow < ROWS;
}


/*inline TSize 
Vga::TPos::ToIndex() const
{
    return mRow * COLS + mCol;
}*/


inline Vga::Pos& 
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


inline Vga::Pos 
Vga::Pos::operator ++(int)
{
    Pos pos = *this;
    ++*this;
    return pos;
}


inline Vga::Pos& 
Vga::Pos::operator =(const Pos& pos)
{
    mCol    = pos.mCol;
    mRow    = pos.mRow;
    return *this;
}


inline 
Vga::Pos::operator Index() const
{
    return mRow * COLS + mCol;
}



#endif  // _VGA_H_
