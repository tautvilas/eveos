#ifndef __SYSTEM_H
#define __SYSTEM_H

#define KERNEL_CALL __attribute__((cdecl))

typedef unsigned char   byte_t;
typedef unsigned short  word_t;
typedef unsigned int    dword_t;

/* MAIN.C */
extern byte_t* KERNEL_CALL
memcpy(byte_t* apDest, const byte_t* apSrc, int aCount);

extern byte_t* KERNEL_CALL
memset(byte_t* apDest, byte_t aVal, int aCount);

extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int strlen(const char *str);
extern unsigned char inportb(unsigned short _port);
extern void outportb(unsigned short _port, unsigned char _data);
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();

extern void KERNEL_CALL
os_main();

#endif
