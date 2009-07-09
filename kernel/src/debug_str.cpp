#include <debug_str.h>
#include <out_int.h>

void 
debugDump(OutStream& out, const char* str)
{
    out << '"' << str << "\"\t("
            << OutInt(str).base(HEX).prefix(false).pad(true)
            << ")";
}


void
debugDump(OutStream& out, unsigned char c)
{
    out << '\'' << c << "\'"
            << "\t(" << OutInt(c).base(HEX).prefix(true).pad(false)
            << ", " << OutInt(c).base(BIN).prefix(false).pad(true)
            << ", " << OutInt(c).base(DEC)
            << ")";
}
