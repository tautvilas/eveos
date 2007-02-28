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


extern word_t* KERNEL_CALL
memsetw(word_t* apDest, word_t aVal, size_t aCount)
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

    //vga_clear();
    vga_set_cursor_pos(0, 4);
    vga_cl_print("Eve successfully switched to P-mode\n", VGA_CL_WHITE, vga_get_bg_color());
    vga_show_cursor(FALSE);
    vga_set_cursor_pos(0, 10);
    vga_set_bg_color(VGA_CL_BLACK);
    vga_set_fg_color(VGA_CL_RED);
    vga_print("\t\t\twelcome\n");
    vga_cl_print("\t\t\t\tto the\n", VGA_CL_BROWN, vga_get_bg_color());
    vga_cl_print("\t\t\t\t\tvga.c world", VGA_CL_YELLOW, vga_get_bg_color());
    vga_print("!\n");

    //for (;;);
    return;
}
