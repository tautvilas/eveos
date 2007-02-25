#include <system.h>

/* copy 'count' bytes of data from 'src' to 'dest' */
byte_t* KERNEL_CALL
        memcpy(byte_t* apDest, const byte_t* apSrc, int aCount)
{
    for(; aCount; aCount--)
    {
        *apDest++ = *apSrc++;
    }
    return apDest;
}

/* set 'count' bytes in 'dest' to 'val' */
extern byte_t* KERNEL_CALL
memset(byte_t* apDest, byte_t aVal, int aCount)
{
    for( ; aCount; aCount--)
    {
        *apDest++ = aVal;
    }
    return apDest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
    return 0;
}

int strlen(const char *str)
{
    const char* p = str;
    while (*p) ++p;
	return p - str;
}

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void KERNEL_CALL
os_main()
{
    //gdt_install();

    byte_t* pVga    = (byte_t*)0xB8000;
    pVga[0]         = 'X';

    byte_t b = 'A';
    memcpy(pVga , &b, 1);

    //for (;;);
    return;
}
