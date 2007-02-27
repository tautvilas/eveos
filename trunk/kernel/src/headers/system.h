#ifndef __SYSTEM_H
#define __SYSTEM_H

#define KERNEL_CALL //__attribute__((cdecl))

typedef unsigned char   byte_t;
typedef unsigned short  word_t;
typedef unsigned int    dword_t;
typedef unsigned int    size_t;


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
 *  Counts zero-terminated string length.
 *
 *  @param  apStr   String address.
 *  @return         Number of bytes from apStr to first zero byte not counting
 *                  zero byte.
 */
extern size_t KERNEL_CALL
strlen(const char *apStr);

/**
 *  Returns byte of data from specified hardware port.
 *
 *  @param  aPort   Port to get byte from.
 *  @return         Byte of data from specified port.
 */
extern byte_t KERNEL_CALL
inportb(word_t aPort);

/**
 *  Sends byte to specified hardware port.
 *
 *  @param  aPort   Port to send byte to.
 *  @param  aByte   Byte to send.
 */
extern void
outportb(word_t aPort, byte_t aByte);

//extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
//extern void gdt_install();

/**
 *  EveOS kernel main function.
 *
 *  First entry point to C.
 */
extern void KERNEL_CALL
os_main();

#endif
