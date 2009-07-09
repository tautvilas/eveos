#include <debug_int.h>
#include <out_str.h>

void 
debugDump(OutStream& out, Int i)
{
    out << "[Int]\t" << OutInt(i).base(HEX).prefix(true).pad(true)
            << " " << OutInt(i).base(BIN).prefix(true).pad(true)
            << " " << i;
}


void
debugDump(OutStream& out, UInt i)
{
    out << "[UInt]\t" << OutInt(i).base(HEX).prefix(true).pad(true)
            << " " << OutInt(i).base(BIN).prefix(true).pad(true)
            << " " << i;
}


void 
debugDump(OutStream& out, const void* p)
{
    out << "[Addr]\t" << OutInt(p).base(HEX).prefix(true).pad(true)
            << " " << OutInt(p).base(BIN).prefix(true).pad(true);
}

