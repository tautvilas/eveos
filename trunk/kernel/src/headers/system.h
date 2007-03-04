#ifndef _SYSTEM_H
#define _SYSTEM_H

/******************************************************************************
 *  Global types, macros and constants
 ******************************************************************************/

#define KERNEL_CALL //__attribute__((cdecl))


typedef unsigned char   bool_t;
typedef unsigned char   byte_t;
typedef unsigned short  word_t;
typedef unsigned int    dword_t;
typedef unsigned int    size_t;

#define FALSE   0
#define TRUE    1

/******************************************************************************/



/******************************************************************************/

/**
 *  EveOS kernel main function.
 *
 *  First entry point to C.
 *
 *  gd 2007-02-28: I think it shouldn't be here though it shouldn't be ever
 *      called from C.
 */
extern void KERNEL_CALL
os_main();

/******************************************************************************/


/******************************************************************************
 *  Memory manupulation functions (should be mem.c)
 ******************************************************************************/

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
 *  Counts zero-terminated string length.
 *
 *  @param  apStr   String address.
 *  @return         Number of bytes from apStr to first zero byte not counting
 *                  zero byte.
 */
extern size_t KERNEL_CALL
strlen(const char *apStr);

/******************************************************************************
 *  I/O ports functions and types (should be lowlevel.c)
 ******************************************************************************/
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

/******************************************************************************/

//extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
//extern void gdt_install();





/******************************************************************************
 *  VGA manipulation functions, types, etc. (vga.c)
 ******************************************************************************/

#define VGA_CL_BLACK                        0
#define VGA_CL_BLUE	                        1
#define VGA_CL_GREEN                        2
#define VGA_CL_CYAN                         3
#define VGA_CL_RED                          4
#define VGA_CL_MAGENTA                      5
#define VGA_CL_BROWN                        6
#define VGA_CL_LIGHT_GRAY                   7
#define VGA_CL_DARK_GRAY                    8
#define VGA_CL_LIGHT_BLUE                   9
#define VGA_CL_LIGHT_GREEN                  10
#define VGA_CL_LIGHT_CYAN                   11
#define VGA_CL_LIGHT_RED                    12
#define VGA_CL_LIGHT_MAGENTA                13
#define VGA_CL_YELLOW                       14
#define VGA_CL_WHITE                        15

typedef byte_t  color_t;

typedef struct
{
    byte_t  mX;
    byte_t  mY;
} vga_pos_t;


extern void KERNEL_CALL
vga_print(const char* apStr);

extern void KERNEL_CALL
vga_cl_print(const char* apStr, color_t aFgColor, color_t aBgColor);

extern void KERNEL_CALL
vga_print_char(char aChar);

extern void KERNEL_CALL
vga_clear();

extern void KERNEL_CALL
vga_set_fg_color(color_t aColor);

extern void KERNEL_CALL
vga_set_bg_color(color_t aColor);

extern color_t KERNEL_CALL
vga_get_bg_color();

extern color_t KERNEL_CALL
vga_get_bg_color();

extern void KERNEL_CALL
vga_show_cursor(bool_t aShow);

extern bool_t KERNEL_CALL
vga_cursor_visible();

extern void KERNEL_CALL
vga_set_cursor_pos(byte_t aX, byte_t aY);

extern vga_pos_t KERNEL_CALL
vga_get_cursor_pos();

/******************************************************************************
 * IDT.C
 ******************************************************************************/

#include <idt.h>
//#include <logo.h>

/******************************************************************************/

#endif
