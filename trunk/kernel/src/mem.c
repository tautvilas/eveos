
#include "mem.h"


byte_t* KERNEL_CALL
memcpy(byte_t* apDest, const byte_t* apSrc, size_t aCount)
{
    for(; aCount; aCount--)
    {
        *apDest++ = *apSrc++;
    }
    return apDest;
}


extern byte_t* KERNEL_CALL
memset(byte_t* apDest, byte_t aVal, size_t aCount)
{
    byte_t* pDest   = apDest;
    for(; aCount; aCount--)
    {
        *pDest++ = aVal;
    }
    return apDest;
}


extern word_t* KERNEL_CALL
memsetw(word_t* apDest, word_t aVal, size_t aCount)
{
    word_t* pDest   = apDest;
    for(; aCount; aCount--)
    {
        *pDest++ = aVal;
    }
    return apDest;
}


extern dword_t* KERNEL_CALL
memsetd(dword_t* apDest, dword_t aVal, size_t aCount)
{
    dword_t* pDest  = apDest;
    for(; aCount; aCount--)
    {
        *pDest++ = aVal;
    }
    return apDest;
}


size_t KERNEL_CALL
strlen(const char *apStr)
{
    const char* pPos = apStr;
    for (; *pPos; ++pPos);    // NO BODY
	return pPos - apStr;
}
