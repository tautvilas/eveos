#include <stdio.h>
//#include <global.h>
#include <string.h>

#define COMMAND_BUFFER_SIZE 256

//char gCommandBuffer[COMMAND_BUFFER_SIZE];

static char gspDelims[] = " \n";


void
dump_mem(const void* apMem, size_t aSize);


void main(void) {
    printf("\nEveOS shell v.0.1 welcomes you\n");

    const size_t LINE_SIZE  = 512;
    char pLine[LINE_SIZE];

    while (1)
    {
        printf("> ");
        /*int i = 0;
        char c = getchar();
        while(c != '\n')
        {
            gCommandBuffer[i] = c;
            if (i < COMMAND_BUFFER_SIZE - 1) i++;
            c = getchar();
        }
        gCommandBuffer[i] = 0;
        */
        if (fgets(pLine, LINE_SIZE, stdin))
        {
            printf(pLine);
            //printf("\n");

            char* pCmd  = strtok(pLine, gspDelims);
            if (0 == strcmp(pCmd, "dump"))
            {

            }
        }
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


void
dump_mem(const void* apMem, size_t aSize)
{
    const size_t    GROUP_SIZE  = 4;
    const size_t    LINE_SIZE   = 2;

    size_t i;
    for (i = 0; i < aSize; ++i)
    {
        if (i)
        {
            if (0 == i % (LINE_SIZE * GROUP_SIZE))
                printf("\n");
            else if (0 == i % GROUP_SIZE)
                printf("- ");
        }
        printf("%x ", (*(unsigned char*)apMem++));
    }
}

