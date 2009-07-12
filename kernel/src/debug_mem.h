#ifndef _DEBUG_MEM_H_
#define _DEBUG_MEM_H_

#include <types.h>
#include <out_stream.h>

void
debugDump(OutStream& out, const void*, const void*);

void
debugDump(OutStream& out, const void*, Size);

#endif // _DEBUG_MEM_H_
