#ifndef _STDIO_H_
#define _STDIO_H_

/**
 *  Classic C printf, but not fully functional.
 *
 *  Currently supported formatting options:
 *  %d - int
 *  %x - int in hex format
 */

extern void __attribute__ ((stdcall))
printf(const char* apFormatStr, ...);

/**
 *  print out a character
 *
 *  @param  aC  character to be printed
 */

extern void
putc(char aC);

#endif // _STDIO_H_
