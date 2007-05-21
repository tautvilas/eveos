#ifndef _MEM_H_
#define _MEM_H_

#include "global.h"


/**
 *  Copies bytes from source to destination address.
 *
 *  @param  apDest  Destination address.
 *  @param  apSrc   Source address.
 *  @param  aCount  Number of bytes to copy.
 *  @return         Destination address.
 */
extern byte_t* KERNEL_CALL
memcpy(byte_t* apDest, const byte_t* apSrc, size_t aCount);

/**
 *  Fills memory bytes with specified value.
 *
 *  @param  apDest  Destination address.
 *  @param  aVal    Value to set.
 *  @param  aCount  Number of bytes to fill.
 *  @return         Destination address.
 */
extern byte_t* KERNEL_CALL
memset(byte_t* apDest, byte_t aVal, size_t aCount);


/**
 *  Fills memory words with specified value.
 *
 *  @param  apDest  Destination address.
 *  @param  aVal    Value to set.
 *  @param  aCount  Number of words to fill.
 *  @return         Destination address.
 */
extern word_t* KERNEL_CALL
memsetw(word_t* apDest, word_t aVal, size_t aCount);


/**
 *  Fills memory double words with specified value.
 *
 *  @param  apDest  Destination address.
 *  @param  aVal    Value to set.
 *  @param  aCount  Number of double words to fill.
 *  @return         Destination address.
 */
extern dword_t* KERNEL_CALL
memsetd(dword_t* apDest, dword_t aVal, size_t aCount);


/**
 *  Counts zero-terminated string length.
 *
 *  @param  apStr   String address.
 *  @return         Number of bytes from apStr to first zero byte not counting
 *                  zero byte.
 */
extern size_t KERNEL_CALL
strlen(const char *apStr);

#endif // _MEM_H_
