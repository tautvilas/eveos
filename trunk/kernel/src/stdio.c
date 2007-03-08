#include "vga.h"
#include "global.h"

#define PRINT vga_print
#define PRINT_CHAR vga_print_char

char gHexTable[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void KERNEL_CALL
print_int_dec(const int aInt)
{
    int x = aInt;
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

void KERNEL_CALL
print_int_hex(const int aInt)
{
    int hex_size = 10;
    int x = aInt;
    int i;
    char number[hex_size];
    for(i = 0; i < hex_size; i++) number[i] = '0';
    number[1] = 'x';
    i = 2;
    int count = 0;
    while(i < hex_size && x)
    {
        number[i] = gHexTable[x % 16];
        x /= 16;
        count++;
    }
    for(i = 0; i < count+2; i++)
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
