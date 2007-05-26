#include "global.h"
#include "mem.h"

#define PRINT_STRING    sys_print_string
#define PRINT_CHAR      sys_print_char

static char gHexTable[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
static char number[256];

static void
sys_print_string(char* apString)
{
    __asm__ __volatile__ ("movl $4, %eax");                                     // syscall id (sys_write)
    __asm__ __volatile__ ("movl $1, %ebx");                                     // stdio
    __asm__ __volatile__ ("movl %0, %%ecx;" :: "m"(apString) : "%ecx");         // string offset
    __asm__ __volatile__ ("movl %0, %%edx;" :: "D"(strlen(apString)) : "edx");  // string length
    __asm__ __volatile__ ("int $69");
    return;
}

static void
sys_print_char(char c)
{
    __asm__ __volatile__ ("movl $4, %eax");
    __asm__ __volatile__ ("movl $1, %ebx");
    __asm__ __volatile__ ("movl %0, %%ecx;" :: "D"(&c) : "%ecx");
    __asm__ __volatile__ ("movl $1, %edx;");
    __asm__ __volatile__ ("int $69");
}

static void
print_int_dec(const int aInt)
{
    int x = aInt;

    if (x == 0)
    {
        PRINT_CHAR('0');
        return;
    }

    int y = 0;
    int count = 0;

    if(x < 0) {
        PRINT_CHAR('-');
        x *= -1;
    }
    while(x) {
        y *= 10;
        y += x%10;
        x /= 10;
        count++;
    }

    while(count) {
        count--;
        PRINT_CHAR(y%10 + '0');
        y /= 10;
    }
    return;
}

static void
print_int_hex(const unsigned int aInt)
{
    unsigned int x = aInt;
    int i;
    int count = 0;
    while(x)
    {
        number[count] = gHexTable[x % 16];
        x /= 16;
        count++;
    }
    PRINT_STRING("0x");
    if(count == 0) PRINT_CHAR('0');
    for(i = count-1; i >= 0; i--)
    {
        PRINT_CHAR(number[i]);
    }
    return;
}

void __attribute__((stdcall))
printf(char * apFormatStr, ...)
{
    char *pS;
    void * pArgs;
    int * int_val;
    pArgs = &apFormatStr;
    //void *pArgs = apFormatStr + ;
    for(pS = apFormatStr; *pS; pS++)
    {
        if(*pS != '%')
        {
            PRINT_CHAR(*pS);
            continue;
        }
        switch (*++pS) {
            case 'd':
                pArgs = (dword_t *) pArgs + 1;
                int_val = (int *)pArgs;
                print_int_dec(*int_val);
                break;
            case 'x':
                pArgs = (dword_t *) pArgs + 1;
                int_val = (int *)pArgs;
                print_int_hex(*int_val);
                break;
            default:
                PRINT_CHAR(*pS);
                break;
        }
    }
    /* TODO stack cleanup? */
    return;
}

void
putc(char aChar)
{
	PRINT_CHAR(aChar);
	return;
}
