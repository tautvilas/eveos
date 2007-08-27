#ifndef _VGA_H_
#define _VGA_H_

#include "global.h"
#include "io_port.h"



class Vga
{
public:

    enum TColor
    {
        BLACK                   = 0,
        BLUE                    = 1,
        GREEN                   = 2,
        CYAN                    = 3,
        RED                     = 4,
        MAGENTA                 = 5,
        BROWN                   = 6,
        LIGHT_GRAY              = 7,
        DARK_GRAY               = 8,
        LIGHT_BLUE              = 9,
        LIGHT_GREEN             = 10,
        LIGHT_CYAN              = 11,
        LIGHT_RED               = 12,
        LIGHT_MAGENTA           = 13,
        YELLOW                  = 14,
        WHITE                   = 15,

        DEFAULT_FG              = LIGHT_GRAY,
        DEFAULT_BG              = BLACK,
    };


    class TPos
    {
    public:

        KERNEL_CALL
        TPos();

        KERNEL_CALL
        TPos(TSize aCol, TSize aRow);

        KERNEL_CALL
        TPos(const TPos& aPos);

        const TSize KERNEL_CALL
        Col() const;

        const void KERNEL_CALL
        Col(TSize aCol);

        const TSize KERNEL_CALL
        Row() const;

        const void KERNEL_CALL
        Row(TSize aRow);

        bool KERNEL_CALL
        IsValid() const;

        TPos& KERNEL_CALL
        operator ++();              // prefix

        TPos KERNEL_CALL
        operator ++(int);           // postfix

        TPos& KERNEL_CALL
        operator =(const TPos& aPos);

        // :TODO: gx 7/28/2007: other operators


    private:

        TSize mCol;
        TSize mRow;

        KERNEL_CALL
        TPos(TWord aIndex);

        //TSize KERNEL_CALL
        //ToIndex() const;

        KERNEL_CALL
        operator TSize() const;

        friend class Vga;
        //friend class Vga::Caret;
    };


    /**
     *  Static class to maipulate VGA caret.
     */
    class Caret
    {
    public:

        /**
         *  Returns ceret visibility.
         *
         *  @return true if caret visible, false otherwise.
         */
        static bool KERNEL_CALL
        Visible();

        /**
         *  Changes caret visibility.
         *
         *  @param  aVisible    true to male caret visible, false to hide caret.
         */
        static void KERNEL_CALL
        Visible(bool aVisible);

        /**
         *  Returns caret position.
         *
         *  @return Caret position.
         */
        static TPos KERNEL_CALL
        Pos();

        /**
         *  Changes caret position.
         *
         *  @param  aPos    New caret position.
         */
        static void KERNEL_CALL
        Pos(const TPos& aPos);


    private:

        /**
         *  Class representing VGA caret register.
         */
        class TReg
        {
        public:

            enum TRegIndex
            {
                POS_HI      = 0x0E,
                POS_LO      = 0x0F,
                VISIBILITY  = 0x0A,     // actually this is "caret start
                                        // register" but as we use it only
                                        // for visibility lets name it this
                                        // way
            };

            /**
             *  COnstructor.
             *
             *  @param  aRegIndex VGA caret register index.
             */
            KERNEL_CALL
            TReg(TRegIndex aRegIndex);

            /**
             *  Reads and returns TByte of data from register.
             *
             *  @return TByte of data from register.
             */
            TByte KERNEL_CALL
            Read();

            /**
             *  Writes TByte of data to register.
             *
             *  @param  aData   Data to write to register.
             */
            void KERNEL_CALL
            Write(TByte aData);

        private:

            enum
            {
                ADDR_PORT   = 0x3d4,
                DATA_PORT   = 0x3d5,
            };

            // :TODO: gx 7/29/2007: make static when static/global constructor
            // calls are implemented
            static TIoPort  msAddrPort;
            static TIoPort  msDataPort;

            TRegIndex       mRegIndex;
        };


        enum
        {
            HIDE_MASK     = 0x20,     // 00100000b
        };

        KERNEL_CALL
        Caret();
    };


    static void KERNEL_CALL
    Put(char aChar, const TPos& aPos, TColor aFgColor, TColor aBgColor);

    static void KERNEL_CALL
    Put(char aChar, const TPos& aPos);


    static void KERNEL_CALL
    Put(const char* apStr, const TPos& aPos, TColor aFgColor, TColor aBgColor);

    static void KERNEL_CALL
    Put(const char* apStr, const TPos& aPos);


    //static void KERNEL_CALL
    //Put(int aInt, TIntegerBase aBase, const TPos& aPos, TColor aFgColor,
    //        TColor aBgColor);

    //static void KERNEL_CALL
    //Put(int aInt, TIntegerBase aBase, const TPos& aPos);


    static void KERNEL_CALL
    Print(char aChar, TColor aFgColor, TColor aBgColor);

    static void KERNEL_CALL
    Print(const char aChar);


    static void KERNEL_CALL
    Print(const char* apStr, TColor aFgColor, TColor aBgColor);

    static void KERNEL_CALL
    Print(const char* apStr);


    //static void KERNEL_CALL
    //Print(int aInt, TIntegerBase aBase, TColor aFgColor, TColor aBgColor);

    //static void KERNEL_CALL
    //Print(int aInt, TIntegerBase aBase);



    static const TColor KERNEL_CALL
    Foreground();

    static const void KERNEL_CALL
    Foreground(TColor aColor);


    static const TColor KERNEL_CALL
    Background();

    static const void KERNEL_CALL
    Background(TColor aColor);


private:

    typedef TWord       TChar;

    static TChar* const MEM_BEGIN;
    static TChar* const MEM_END;

    static const TSize  COLS;
    static const TSize  ROWS;
    static const TSize  TAB_WIDTH;
    static const char   BLANK_CHAR;


    static TColor       msBgColor;
    static TColor       msFgColor;

    KERNEL_CALL
    Vga();

    static TChar KERNEL_CALL
    Char(char aChar, TColor aFg, TColor aBg);

    static void KERNEL_CALL
    ScrollUp(TSize aLines = 1);

    static char* KERNEL_CALL
    IntToStr(int aInt, TIntegerBase aBase);

    static char* KERNEL_CALL
    UIntToStr(unsigned int aInt, TIntegerBase aBase, bool aNegative);

    //friend Pos;
};




////// Vga inlines //////


/*static*/ inline void KERNEL_CALL
Vga::Put(char aChar, const TPos& aPos)
{
    Put(aChar, aPos, msFgColor, msBgColor);
}


/*static*/ inline void KERNEL_CALL
Vga::Put(const char* apStr, const TPos& aPos)
{
    Put(apStr, aPos, msFgColor, msBgColor);
}


///*static*/ inline void KERNEL_CALL
//Vga::Put(int aInt, TIntegerBase aBase, const TPos& aPos)
//{
//    Put(aInt, aBase, aPos, msFgColor, msBgColor);
//}


/*static*/ inline void KERNEL_CALL
Vga::Print(const char aChar)
{
    Print(aChar, msFgColor, msBgColor);
}


/*static*/ inline void KERNEL_CALL
Vga::Print(const char* apStr)
{
    Print(apStr, msFgColor, msBgColor);
}


///*static*/ inline void KERNEL_CALL
//Vga::Print(int aInt, TIntegerBase aBase)
//{
//    Print(aInt, aBase, msFgColor, msBgColor);
//}


/*static*/ inline const Vga::TColor KERNEL_CALL
Vga::Foreground()
{
    return msFgColor;
}


/*static*/ inline const void KERNEL_CALL
Vga::Foreground(TColor aColor)
{
    msFgColor   = aColor;
}


/*static*/ inline const Vga::TColor KERNEL_CALL
Vga::Background()
{
    return msBgColor;
}


/*static*/ inline const void KERNEL_CALL
Vga::Background(TColor aColor)
{
    msBgColor   = aColor;
}


/*static*/ inline Vga::TChar KERNEL_CALL
Vga::Char(char aChar, TColor aFg, TColor aBg)
{
    return static_cast<TChar>(aChar) | aFg << 8 | aBg << 12;
}



////// Vga::Caret::TReg inlines //////

inline KERNEL_CALL
Vga::Caret::TReg::TReg(TRegIndex aRegIndex)
        : mRegIndex(aRegIndex)
{
}


inline void KERNEL_CALL
Vga::Caret::TReg::Write(TByte aData)
{
    msAddrPort.WriteByte(mRegIndex);
    msDataPort.WriteByte(aData);
}


inline TByte KERNEL_CALL
Vga::Caret::TReg::Read()
{
    msAddrPort.WriteByte(mRegIndex);
    return msDataPort.ReadByte();
}


////// Vga::TPos inlines //////

inline KERNEL_CALL
Vga::TPos::TPos()
        : mCol(0), mRow(0)
{
}


inline KERNEL_CALL
Vga::TPos::TPos(TSize aCol, TSize aRow)
        : mCol(aCol), mRow(aRow)
{
}


inline KERNEL_CALL
Vga::TPos::TPos(const TPos& aPos)
        : mCol(aPos.mCol), mRow(aPos.mRow)
{
}


inline KERNEL_CALL
Vga::TPos::TPos(TWord aIndex)
        : mCol(aIndex % COLS), mRow(aIndex / COLS)
{
}


inline bool KERNEL_CALL
Vga::TPos::IsValid() const
{
    return mCol < COLS && mRow < ROWS;
}


/*inline TSize KERNEL_CALL
Vga::TPos::ToIndex() const
{
    return mRow * COLS + mCol;
}*/


inline Vga::TPos& KERNEL_CALL
Vga::TPos::operator ++()
{
    ++mCol;
    if (mCol >= COLS)
    {
        mCol    = 0;
        ++mRow;
    }
    return *this;
}


inline Vga::TPos KERNEL_CALL
Vga::TPos::operator ++(int)
{
    TPos pos = *this;
    ++*this;
    return pos;
}


inline Vga::TPos& KERNEL_CALL
Vga::TPos::operator =(const TPos& aPos)
{
    mCol    = aPos.mCol;
    mRow    = aPos.mRow;
    return *this;
}


inline KERNEL_CALL
Vga::TPos::operator TSize() const
{
    return mRow * COLS + mCol;
}



#endif  // _VGA_H_

