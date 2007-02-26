#include <system.h>

/* copy 'count' bytes of data from 'src' to 'dest' */
byte_t* KERNEL_CALL
memcpy(byte_t* apDest, const byte_t* apSrc, size_t aCount)
{
    for(; aCount; aCount--)
    {
        *apDest++ = *apSrc++;
    }
    return apDest;
}

/* set 'count' bytes in 'dest' to 'val' */
extern byte_t* KERNEL_CALL
memset(byte_t* apDest, byte_t aVal, size_t aCount)
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

    byte_t  s1[] = { 'A', 0, 'B', 0, 'C', 0 };
    byte_t  s2[] = "B\0B\0C";
    memset(pVga, '-', 6);
    memcpy(pVga, s1, 1);
    memcpy(pVga + 80*2, s2, 6);


    //for (;;);
    return;
}
