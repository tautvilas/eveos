#include <global.h>


extern "C" Byte gKernelBase;
extern "C" Byte gKernelOffset;
extern "C" Byte gKernelStart;
extern "C" Byte gKernelEnd;


const Addr Kernel::BASE     = addr(&gKernelBase);
const Addr Kernel::START    = addr(&gKernelStart);
const Addr Kernel::END      = addr(&gKernelEnd);

