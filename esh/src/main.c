#include <stdio.h>
//#include <global.h>
#include <string.h>
#include <stdlib.h>
#include <syscalls.h>

#define COMMAND_BUFFER_SIZE 256

// char gCommandBuffer[COMMAND_BUFFER_SIZE];

// static char gspDelims[] = " \n";

void
dump_mem(const void* apMem, size_t aSize);

void
dump_regs();

void
exec_program(const char* apFileName, priority_t aPriority, int aOnTop);

void
show_help();

char**
parse_cmd(char* apCmd);

int
arg_count(char** apCmd);

void
err_arg_count();

void
err_cmd();

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
        //DUMP(i);
        gCommandBuffer[i] = 0;
        printf("\n");
        printf(gCommandBuffer);
        printf("\n");*/

        if (fgets(pLine, LINE_SIZE, stdin))
        {
            //printf(pLine);
            //printf("\n");

            char** pCmd  = parse_cmd(pLine);

            if (0 == strcmp(pCmd[0], "mem"))
            {
                if (2 != arg_count(pCmd))
                    err_arg_count();
                else
                    dump_mem((void*)atoi(pCmd[1]), atoi(pCmd[2]));
            }
            else if (0 == strcmp(pCmd[0], "regs"))
            {
                if (0 != arg_count(pCmd))
                    err_arg_count();
                else
                    dump_regs();
            }
            else if (0 == strcmp(pCmd[0], "exec"))
            {
                if (3 != arg_count(pCmd))
                    err_arg_count();
                else
                    exec_program(pCmd[1], atoi(pCmd[2]), atoi(pCmd[3]));
            }
            else if (0 == strcmp(pCmd[0], "help"))
            {
                if (0 != arg_count(pCmd))
                    err_arg_count();
                else
                    show_help();
            }
            else
            {
                err_cmd();
            }

        }
    }

    for (;;);
}



char**
parse_cmd(char* apCmd)
{
    const int       CMD_SIZE    = 10;
    static char*    spCmd[10];
    static char     spDelims[]  = " \n\t";

    spCmd[0] = strtok(apCmd, spDelims);
    while (spCmd[0] && (0 == strcmp("", spCmd[0])))
        spCmd[0] = strtok(NULL, spDelims); // NO BODY

    int i = 1;
    while (i < CMD_SIZE && spCmd[i - 1])
    {
        while (
                ((spCmd[i] = strtok(NULL, spDelims)) != NULL)
                && (0 == strcmp("", spCmd[i]))
            ); // NO BODY
        ++i;
    }
    return spCmd;
}


int
arg_count(char** apCmd)
{
    int c;
    for (c = 0; apCmd[c]; ++c); // NO BODY
    return c - 1;
}


void err_arg_count()
{
    printf("bad arg count!\n");
}


void err_cmd()
{
    printf("unknown command!\n");
}


void
show_help()
{
    printf(
            "Commands:\n"
            "\tmem <address> <size>\n"
            "\tregs\n"
            "\texec <filename> <priority> <ontop?>\n"
            "\n"
        );
}


void
dump_mem(const void* apMem, size_t aSize)
{
    const size_t    GROUP_SIZE  = 4;
    const size_t    LINE_SIZE   = 2;

    printf("Dumping %d bytes of memory at address %x:\n", aSize, apMem);

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
    printf("\n");
}

void
dump_regs()
{
    struct
    {
        dword_t gs, fs, es, ds, cs, ss;
        dword_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    } regs;
    __asm__ __volatile__ ("movl %%eax, %0\n" :"=r" (regs.eax));
    __asm__ __volatile__ ("movl %%ebx, %0\n" :"=r" (regs.ebx));
    __asm__ __volatile__ ("movl %%ecx, %0\n" :"=r" (regs.ecx));
    __asm__ __volatile__ ("movl %%edx, %0\n" :"=r" (regs.edx));
    __asm__ __volatile__ ("movl %%esp, %0\n" :"=r" (regs.esp));
    __asm__ __volatile__ ("movl %%ebp, %0\n" :"=r" (regs.ebp));
    __asm__ __volatile__ ("movl %%edi, %0\n" :"=r" (regs.edi));
    __asm__ __volatile__ ("movl %%esi, %0\n" :"=r" (regs.esi));
    __asm__ __volatile__ ("movl %%ds, %0\n" :"=r" (regs.ds));
    __asm__ __volatile__ ("movl %%es, %0\n" :"=r" (regs.es));
    __asm__ __volatile__ ("movl %%fs, %0\n" :"=r" (regs.fs));
    __asm__ __volatile__ ("movl %%gs, %0\n" :"=r" (regs.gs));
    __asm__ __volatile__ ("movl %%ss, %0\n" :"=r" (regs.ss));
    __asm__ __volatile__ ("movl %%cs, %0\n" :"=r" (regs.cs));
    printf("eax:%x\tebx:%x\tecx:%x\tedx:%x\n", regs.eax, regs.ebx, regs.ecx, regs.edx);
    printf("esp:%x\tebp:%x\tedi:%x\tesi:%x\n", regs.esp, regs.ebp, regs.edi, regs.esi);
    printf(" cs:%x\t ds:%x\t ss:%x\n", regs.cs, regs.ds, regs.ss);
    printf(" es:%x\t fs:%x\t gs:%x\n", regs.es, regs.fs, regs.gs);
}


void
exec_program(const char* apFileName, priority_t aPriority, int aOnTop)
{
    const char*    LEVELS[]  = { "low", "normal", "high" };

    aPriority   %= 3;

    printf("Executing program '%s' ", apFileName);
    if (aOnTop)
        printf("on top ");
    else
        printf("in background ");
    printf("with %s priority level...\n", LEVELS[aPriority]);

    if (0 != exec(apFileName, aPriority, aOnTop))
    {
        printf("Error occured!\n");
    }
}

