#include <stdio.h>
#include <global.h>

int main(void) {
    for(;;)
    {
        int i;
        for(i = 0; i < 300000000; i++);
        dword_t esp;
        __asm__ __volatile__ ("movl %%esp, %0\n" :"=r" (esp));
        printf("### PING! (esp:%x) ###\n", esp);
    }
}
