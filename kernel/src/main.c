#include <system.h>

/* copy 'count' bytes of data from 'src' to 'dest' */
unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count)
{
    char * dp = (char *) dest;
    char * sp = (char *) src;
    for( ; count; count--)
    {
        *dp++ = *sp++;
    }
    return 0;
}

/* set 'count' bytes in 'dest' to 'val' */
unsigned char *memset(unsigned char *dest, unsigned char val, int count)
{
    char *temp = (char *) dest;
    for( ; count; count--)
    {
        *temp++ = val;
    }
    return dest;
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

void os_main()
{
    gdt_install();
    //for (;;);
    return;
}
