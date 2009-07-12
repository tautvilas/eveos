#include <debug_mem.h>
#include <out_int.h>
#include <out_str.h>


namespace {

struct Range
{
    const void* mBegin;
    const void* mEnd;
    Range(const void* begin, const void* end) : mBegin(begin), mEnd(end) {}
};

struct Space
{
    Size mPos;
    Space(Size pos) : mPos(pos) {}
};

struct ByteHex
{
    Byte mVal;
    ByteHex(Byte val) : mVal(val) {}
};

struct ByteAscii
{
    Byte mVal;
    ByteAscii(Byte val) : mVal(val) {}
};

struct Pad
{
    Size mPos;
    Size mLen;
    Pad(Size pos, Size len) : mPos(pos), mLen(len) {}
};

struct Header
{
    Size mWidth;
    Header(Size width) : mWidth(width) {}
};

OutStream&
operator <<(OutStream& out, const Range& range);

OutStream&
operator <<(OutStream& out, const Space&);

OutStream&
operator <<(OutStream& out, const ByteHex&);

OutStream&
operator <<(OutStream& out, const ByteAscii&);

OutStream&
operator <<(OutStream& out, const Pad&);

OutStream&
operator <<(OutStream& out, const Header&);

}   // namespace


void
debugDump(OutStream& out, const void* begin, const void* end)
{
    out << Range(begin, end);
    if (begin > end)
    {
        out << " INVALID";
        return;
    }

    const Size  WIDTH   = 16;

    out << '\n' << Header(WIDTH) << '\n';
     
    for (const Byte* b = static_cast<const Byte*>(begin); b != end;)
    {
        Size shift  = addr(b) % WIDTH;
        Size pos    = shift;
        
        out << OutInt(b - shift).base(HEX).pad(true);
        
        out << Pad(0, shift);        
        for (; b + pos != end && pos < WIDTH; ++pos)
            out << Space(pos) << ByteHex(b[pos - shift]);
        out << Pad(pos, WIDTH);
        
        out << ' ';
        for (Size i = 0; i < shift; ++i)
            out << ' ';
        for (Size i = shift; i < pos; ++i)
            out << ByteAscii(b[i - shift]);
        
        out << '\n';
        b += pos - shift;
    }

    //out << Header();
}


void
debugDump(OutStream& out, const void* begin, Size size)
{
    debugDump(out, begin, reinterpret_cast<const Byte*>(begin) + size);
}


namespace {

OutStream&
operator <<(OutStream& out, const Range& range)
{
    out << '[' << range.mBegin << "; " << range.mEnd << ')';
    return out;
}

OutStream&
operator <<(OutStream& out, const Space& space)
{
    if (0 == space.mPos % 4)
        out << ' ';
    if (0 == space.mPos % 8)
        out << ' ';
    return out;
}

OutStream&
operator <<(OutStream& out, const ByteHex& byte)
{
    if (byte.mVal < HEX) 
        out << '0';
        
    out << OutInt(byte.mVal).base(HEX) << ' ';
    return out;
}

OutStream&
operator <<(OutStream& out, const ByteAscii& byte)
{
    char c = byte.mVal; 
    out << (c >= 32 && c < 127 ? c : '.');
    return out;
}

OutStream&
operator <<(OutStream& out, const Pad& pad)
{
    for (Size pos = pad.mPos; pos < pad.mLen; ++pos)
        out << Space(pos) << "   ";
    return out;
}

OutStream&
operator <<(OutStream& out, const Header& head)
{
    for (Size i = 0; i < sizeof(void*)*2; ++i)
        out << ' ';
    for (Size w = 0; w < head.mWidth;)
        for (Size i = 0; i < HEX && w < head.mWidth; ++i, ++w)
            out << Space(i) << ' ' << OutInt(i).base(HEX) << ' ';
    out << ' ';
    for (Size w = 0; w < head.mWidth;)
        for (Size i = 0; i < HEX && w < head.mWidth; ++i, ++w)
            out << OutInt(i).base(HEX);
    return out;
}

} // namespace

