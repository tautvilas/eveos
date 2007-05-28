#include <stdio.h>
#include <global.h>
//#include <mem.h>

extern void __attribute__((__cdecl)) sys_write(const char* apStr, int len);

size_t //__attribute__((stdcall))
strlen2(const char *apStr)
{
    const char* pPos = apStr;
    for (; *pPos; ++pPos);    // NO BODY
	return pPos - apStr;
}

void main(void) {
    // __asm__ __volatile__ ("cli");
    for(;;)
    {
        int i;
        for(i = 0; i < 4000000000U; i++);
        //dword_t esp;
        //__asm__ __volatile__ ("movl %%esp, %0\n" :"=r" (esp));
        //printf("### PING! (esp:%x) ###\n", esp);
        //printf("Something wicked this way comes\n");
        char * string = "@@@@@@@@@@ Something wicked @@@@@@@@@@\n\0";
        size_t len = strlen2(string);
        sys_write(string, len);
        continue;

        __asm__ __volatile__ ("pusha");
        __asm__ __volatile__ ("movl $4, %eax");                                   // syscall id (sys_write)
        __asm__ __volatile__ ("movl $1, %ebx");                                   // stdio
        __asm__ __volatile__ ("movl %0, %%ecx;" :: "S"(string) : "%ecx");     // string offset

        // something wrong with this statement -- looks like it loads len address to edx
        __asm__ __volatile__ ("movl %0, %%edx;" :: "m"(len) : "edx");  // string length

        //__asm__ __volatile__ ("movl $39, %edx;");  // string length
        __asm__ __volatile__ ("int $69");
        __asm__ __volatile__ ("popa");
    }
    //__asm__ __volatile__ ("sti");
}
