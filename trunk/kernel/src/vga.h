#ifndef _VGA_H_
#define _VGA_H_

#include "global.h"
#include "io_port.h"
#include "mem.h"



class Vga
{
public:

    enum Color
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


    class Position
    {
    public:

        KERNEL_CALL
        Position();

        KERNEL_CALL
        Position(Size aCol, Size aRow);

        KERNEL_CALL
        Position(const Position& aPos);

        const Size KERNEL_CALL
        Col() const;

        const void KERNEL_CALL
        Col(Size aCol);

        const Size KERNEL_CALL
        Row() const;

        const void KERNEL_CALL
        Row(Size aRow);

        bool KERNEL_CALL
        IsValid() const;

        Position& KERNEL_CALL
        operator ++();              // prefix

        Position KERNEL_CALL
        operator ++(int);           // postfix

        Position& KERNEL_CALL
        operator =(const Position& aPos);

        // :TODO: gx 7/28/2007: other operators


    private:

        Size mCol;
        Size mRow;

        KERNEL_CALL
        Position(Word aIndex);

        //Size KERNEL_CALL
        //ToIndex() const;

        KERNEL_CALL
        operator Size() const;

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
        static Position KERNEL_CALL
        Pos();

        /**
         *  Changes caret position.
         *
         *  @param  aPos    New caret position.
         */
        static void KERNEL_CALL
        Pos(const Position& aPos);


    private:

        /**
         *  Class representing VGA caret register.
         */
        class Reg
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
             *  COnstructor.
             *
             *  @param  aRegIndex VGA caret register index.
             */
            KERNEL_CALL
            Reg(RegIndex aRegIndex);

            /**
             *  Reads and returns Byte of data from register.
             *
             *  @return Byte of data from register.
             */
            Byte KERNEL_CALL
            Read();

            /**
             *  Writes Byte of data to register.
             *
             *  @param  aData   Data to write to register.
             */
            void KERNEL_CALL
            Write(Byte aData);

        private:

            enum
            {
                ADDR_PORT   = 0x3d4,
                DATA_PORT   = 0x3d5,
            };

            // :TODO: gx 7/29/2007: make static when static/global constructor
            // calls are implemented
            /*static*/ IoPort   msAddrPort;
            /*static*/ IoPort   msDataPort;

            RegIndex    mRegIndex;
        };


        enum
        {
            HIDE_MASK     = 0x20,     // 00100000b
        };

        KERNEL_CALL
        Caret();
    };


    static void KERNEL_CALL
    Put(char aChar, const Position& aPos, Color aFgColor, Color aBgColor);

    static void KERNEL_CALL
    Put(char aChar, const Position& aPos);


    static void KERNEL_CALL
    Put(const char* apStr, const Position& aPos, Color aFgColor, Color aBgColor);

    static void KERNEL_CALL
    Put(const char* apStr, const Position& aPos);


    static void KERNEL_CALL
    Print(char aChar, Color aFgColor, Color aBgColor);

    static void KERNEL_CALL
    Print(const char aChar);


    static void KERNEL_CALL
    Print(const char* apStr, Color aFgColor, Color aBgColor);

    static void KERNEL_CALL
    Print(const char* apStr);


    static const Color KERNEL_CALL
    Foreground();

    static const void KERNEL_CALL
    Foreground(Color aColor);


    static const Color KERNEL_CALL
    Background();

    static const void KERNEL_CALL
    Background(Color aColor);


private:

    static Word* const  ADDRESS;
    static const Size   COLS;
    static const Size   ROWS;
    static const Size   TAB_WIDTH;
    static const char   BLANK_CHAR;


    static Color        msBgColor;
    static Color        msFgColor;

    KERNEL_CALL
    Vga();

    static Word KERNEL_CALL
    Char(char aChar, Color aFg, Color aBg);

    static void KERNEL_CALL
    ScrollUp(Size aLines = 1);

    //friend Pos;
};




////// Vga inlines //////


/*static*/ inline void KERNEL_CALL
Vga::Put(char aChar, const Position& aPos)
{
    Put(aChar, aPos, msFgColor, msBgColor);
}


/*static*/ inline void KERNEL_CALL
Vga::Put(const char* apStr, const Position& aPos)
{
    Put(apStr, aPos, msFgColor, msBgColor);
}

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


/*static*/ inline const Vga::Color KERNEL_CALL
Vga::Foreground()
{
    return msFgColor;
}


/*static*/ inline const void KERNEL_CALL
Vga::Foreground(Color aColor)
{
    msFgColor   = aColor;
}


/*static*/ inline const Vga::Color KERNEL_CALL
Vga::Background()
{
    return msBgColor;
}


/*static*/ inline const void KERNEL_CALL
Vga::Background(Color aColor)
{
    msBgColor   = aColor;
}


/*static*/ inline Word KERNEL_CALL
Vga::Char(char aChar, Color aFg, Color aBg)
{
    return static_cast<Word>(aChar) | aFg << 8 | aBg << 12;
}



////// Vga::Caret::Reg inlines //////

inline KERNEL_CALL
Vga::Caret::Reg::Reg(RegIndex aRegIndex)
        : msAddrPort(ADDR_PORT), msDataPort(DATA_PORT), mRegIndex(aRegIndex)
{
}


inline void KERNEL_CALL
Vga::Caret::Reg::Write(Byte aData)
{
    msAddrPort.WriteByte(mRegIndex);
    msDataPort.WriteByte(aData);
}


inline Byte KERNEL_CALL
Vga::Caret::Reg::Read()
{
    msAddrPort.WriteByte(mRegIndex);
    return msDataPort.ReadByte();
}


////// Vga::Position inlines //////

inline KERNEL_CALL
Vga::Position::Position()
        : mCol(0), mRow(0)
{
}


inline KERNEL_CALL
Vga::Position::Position(Size aCol, Size aRow)
        : mCol(aCol), mRow(aRow)
{
}


inline KERNEL_CALL
Vga::Position::Position(const Position& aPos)
        : mCol(aPos.mCol), mRow(aPos.mRow)
{
}


inline KERNEL_CALL
Vga::Position::Position(Word aIndex)
        : mCol(aIndex % COLS), mRow(aIndex / COLS)
{
}


inline bool KERNEL_CALL
Vga::Position::IsValid() const
{
    return mCol < COLS && mRow < ROWS;
}


/*inline Size KERNEL_CALL
Vga::Position::ToIndex() const
{
    return mRow * COLS + mCol;
}*/


inline Vga::Position& KERNEL_CALL
Vga::Position::operator ++()
{
    ++mCol;
    if (mCol >= COLS)
    {
        mCol    = 0;
        ++mRow;
    }
    return *this;
}


inline Vga::Position KERNEL_CALL
Vga::Position::operator ++(int)
{
    Position pos = *this;
    ++*this;
    return pos;
}


inline Vga::Position& KERNEL_CALL
Vga::Position::operator =(const Position& aPos)
{
    mCol    = aPos.mCol;
    mRow    = aPos.mRow;
    return *this;
}


inline KERNEL_CALL
Vga::Position::operator Size() const
{
    return mRow * COLS + mCol;
}



#endif  // _VGA_H_

