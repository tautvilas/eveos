#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define KERNEL_CALL __attribute__((stdcall))

typedef unsigned int    Size;
typedef unsigned char   Byte;
typedef unsigned short  Word;
typedef unsigned int    DWord;

extern Byte gKernelBase;


#endif // _GLOBAL_H_

