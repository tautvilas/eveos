#include <global.h>


extern "C" Byte gKernelBase;
extern "C" Byte gKernelOffset;
extern "C" Byte gKernelStart;
extern "C" Byte gKernelEnd;

Byte* const Kernel::BASE    = &gKernelBase;
Byte* const Kernel::START   = &gKernelStart;
Byte* const Kernel::END     = &gKernelEnd;
const Index Kernel::OFFSET  = reinterpret_cast<Index>(&gKernelBase);
