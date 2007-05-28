#include <stdio.h>
#include <global.h>
#include <mem.h>

void main(void) {
    for (;;)
    {
        int i;
        for (i = 0; i < 100000000U; i++);
        printf("########## PING! ##########\n");
    }
}
