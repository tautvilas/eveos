#ifndef _STDIO_H
#define _STDIO_H

/**
 *  Classic C printf, but not fully functional.
 *
 *  @param  apFormatStr  Output format string.
 *  @param  ...          Arguments to be inserted in format string.
 */

/**
 *  Currently support formatting options:
 *  %d - int
 */

extern void __attribute__ ((stdcall))
printf(const char* apFormatStr, ...);

/**
 *  Prints out an integer number
 *
 *  @param  number  number to be printed
 */

extern void KERNEL_CALL
print_int_dec(int number);

/**
 *  Prints out an integer number in hex format
 *
 *  @param  number  number to be printed
 */

extern void KERNEL_CALL
print_int_hex(int number);

#endif
