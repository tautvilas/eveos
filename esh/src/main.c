#include <stdio.h>
#include <global.h>
#include <mem.h>

#define COMMAND_BUFFER_SIZE 256

char gCommandBuffer[COMMAND_BUFFER_SIZE];

void main(void) {
    printf("\nEveOS shell v.0.1 welcomes you\n");

    while (1)
    {
        printf("> ");
        int i = 0;
        char c = getchar();
        while(c != '\n')
        {
            gCommandBuffer[i] = c;
            if (i < COMMAND_BUFFER_SIZE - 1) i++;
            c = getchar();
        }
        gCommandBuffer[i] = 0;
        printf(gCommandBuffer);
        printf("\n");
    }

    for (;;)
    {
        /*
        int i;
        for (i = 0; i < 100000000U; i++);
        printf("########## PING! ##########\n");
        */
    }
}
