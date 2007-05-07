#include "paging.h"
#include "global.h"

#define KB_4    4096
#define S_RW_P  3       /* page flag supervisor level, read/write, present */
#define S_RW_N  2       /* page flag supervisor level, read/write, not present */

extern void write_cr0(dword_t);
extern void write_cr3(dword_t);
extern dword_t read_cr0();
extern dword_t read_cr3();

static dword_t* gpPageDirectory = (dword_t *) 0x1000;   /* page directory address */
static dword_t* gpPageTable = (dword_t *) 0x2000;       /* first page table address */

void KERNEL_CALL
paging_install(void)
{
    unsigned int i;
    dword_t address = 0;

    /* map the first 4MB of memory */
    for(i=0; i<1024; i++)
    {
        /* attribute set to: supervisor level, read/write, present(011 in binary) */
        gpPageTable[i] = address | S_RW_P;
        address = address + KB_4;
    };

    /* fill the first entry of the page directory */
    gpPageDirectory[0] = (dword_t) gpPageTable;
    gpPageDirectory[0] = gpPageDirectory[0] | S_RW_P;

    /* there are no more tables so we fill other entries with "not present" table descriptors */
    for(i=1; i<1024; i++)
    {
        gpPageDirectory[i] = 0 | S_RW_N;
    };

    /* write_cr3, read_cr3, write_cr0, and read_cr0 all come from the assembly functions */
    write_cr3((dword_t) gpPageDirectory);   // put that page directory address into CR3
    write_cr0(read_cr0() | 0x80000000);     // set the paging bit in CR0 to 1
}
