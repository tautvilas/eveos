#include "paging.h"
#include "global.h"
#include "stdio.h"

#define KB_4    4096
#define S_RW_P  3       /* page flag supervisor level, read/write, present */
#define S_RW_N  2       /* page flag supervisor level, read/write, not present */


#define ACC_SUPER           0   
#define ACC_USER            4   // 0100b
#define ACC_READ            0
#define ACC_RW              2   // 0010b
#define ACC_MASK            6   // 0110b

#define ENTRY_PRESENT       1

#define PAGE_SIZE           4096
#define PAGE_TABLE_SIZE     1024
#define PAGE_DIR_SIZE       1024


typedef byte_t          access_t;        
typedef pointer_t       page_table_t[1024];
typedef page_table_t*   page_dir_t[1024];



extern void write_cr0(dword_t);
extern void write_cr3(dword_t);
extern dword_t read_cr0();
extern dword_t read_cr3();


page_table_t* KERNEL_CALL
alloc_page_table(access_t aAccess);

page_dir_t* KERNEL_CALL
alloc_page_directory(access_t aAccess);



//static dword_t* gpPageDirectory = (dword_t *) 0x1000;   /* page directory address */
//static dword_t* gpPageTable = (dword_t *) 0x2000;       /* first page table address */
static page_dir_t*  gpPageDirectory;


void KERNEL_CALL
paging_install(void)
{

    //unsigned int i;
    //dword_t address = 0;

    /* map the first 4MB of memory */
    //for(i=0; i<1024; i++)
    //{
    //    /* attribute set to: supervisor level, read/write, present(011 in binary) */
    //    gpPageTable[i] = address | S_RW_P;
    //    address = address + KB_4;
    //};
    
    size_t          i;
    pointer_t       address;
    page_table_t*   pPageTable;
    
    pPageTable  = alloc_page_table(ACC_SUPER | ACC_RW);
    for (i = 0, address = 0; i < PAGE_TABLE_SIZE; ++i, address += PAGE_SIZE)
        (*pPageTable)[i]    = (pointer_t)((size_t)address | ENTRY_PRESENT);
    

    /* fill the first entry of the page directory */
    //gpPageDirectory[0] = pPageTable;
    //gpPageDirectory[0] = gpPageDirectory[0] | S_RW_P;
    gpPageDirectory         = alloc_page_directory(ACC_SUPER | ACC_RW);
    (*gpPageDirectory)[0]  = (page_table_t*)((size_t)(*gpPageDirectory)[0]
            | (size_t)pPageTable | ENTRY_PRESENT);

    /* there are no more tables so we fill other entries with "not present" table descriptors */
    //for(i=1; i<1024; i++)
    //{
    //    gpPageDirectory[i] = 0 | S_RW_N;
    //};

    /* write_cr3, read_cr3, write_cr0, and read_cr0 all come from the assembly functions */
    write_cr3((dword_t) gpPageDirectory);   // put that page directory address into CR3
    write_cr0(read_cr0() | 0x80000000);     // set the paging bit in CR0 to 1
}


page_table_t* KERNEL_CALL
alloc_page_table(access_t aAccess)
{
    size_t          flags       = aAccess & ACC_MASK;
    page_table_t*   page_table  = (page_table_t*)mm_alloc_page();
    size_t          i;
    for (i = 0; i < PAGE_TABLE_SIZE; ++i)
    {
        (*page_table)[i]    = (pointer_t)flags;
    }
    return page_table;
}


page_dir_t* KERNEL_CALL
alloc_page_directory(access_t aAccess)
{
    size_t          flags       = aAccess & ACC_MASK;
    page_dir_t*     page_dir    = (page_dir_t*)mm_alloc_page();
    size_t          i;
    for (i = 0; i < PAGE_DIR_SIZE; ++i)
    {
        (*page_dir)[i]  = (pointer_t)flags;
    }
    return page_dir;
}


/*pointer_t KERNEL_CALL
kmalloc(size_t size)
{
        
}*/




