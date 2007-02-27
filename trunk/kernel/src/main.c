#include <system.h>


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
    for(; aCount; aCount--)
    {
        *apDest++ = aVal;
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


byte_t KERNEL_CALL
inportb(word_t aPort)
{
    byte_t result;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (result) : "dN" (aPort));
    return result;
}


void
outportb(word_t aPort, byte_t aByte)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (aPort), "a" (aByte));
}


void KERNEL_CALL
os_main()
{
    //gdt_install();

    byte_t* pVga    = (byte_t*)0xB8000;

    byte_t s1[] = { 'E', 2, 'v', 2, 'e', 2, '0', 2, 'S', 2, 0 };
    byte_t * s2 = (byte_t *) "A\07n\07d\07 \07a\07 \07n\07e\07w\07"
        " \07w\07o\07r\07l\07d\07 \07a\07w\07a\07i\07t\07s\07 \07y\07o\07u\07.\06.\05.\04";
    //memset(pVga, '-', 6);
    memcpy(pVga + 80*2*3, s1, strlen(s1));
    memcpy(pVga + 80*2*4, s2, strlen(s2));

    //for (;;);
    return;
}
