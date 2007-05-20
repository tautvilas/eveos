#include "memmgr.h"
#include "stdio.h"
#include "idt.h"

/**
 *  Mask to zero-out unused bits from address
 */
#define MM_PAGE_ADDR_MASK   0xFFFFF000

/**
 *  Number of bits to right-shift page number to get page address
 */
#define MM_PAGE_ADDR_SHIFT  12

/**
 *  RAM size in pages
 *  TODO: load at run time
 */
#define MM_RAM_SIZE         2048

/**
 *  Kernel start page
 *  TODO: load at run time
 */
#define MM_KERNEL_START     8

/**
 *  Kernel size in pages
 *  TODO: load at run time
 */
#define MM_KERNEL_SIZE      6

/**
 *  Values to build access_t value by or'ing
 */
#define ACC_SUPER           0
#define ACC_USER            4   // 0100b
#define ACC_READ            0
#define ACC_RW              2   // 0010b

/**
 *  Bit mask to mask out bits not used by access_t by and'ing
 */
#define ACC_MASK            6   // 0110b

/**
 *  Value to set page table/directory entry as present
 */
#define ENTRY_PRESENT       1


#define MM_PAGE_TABLE_SIZE  1024
#define MM_PAGE_DIR_SIZE    1024


typedef dword_t             mm_access_t;
typedef pointer_t*          mm_page_tbl_t;
typedef mm_page_tbl_t*      mm_page_dir_t;

typedef char                process_name_t[12];

typedef struct {
        process_name_t  mProcName;
        pointer_t       mPointer;
    } mm_page_t;

typedef struct {
        pointer_t       mCode;
        pointer_t       mData;
        pointer_t       mStack;
        pointer_t       mHeap;
        pointer_t       mHeapEnd;
        mm_page_tbl_t   mPageDir[1024];
    } process_mem_t;

// TODO: gx: move to process manager
typedef struct {
        process_mem_t   mMem;
        process_name_t  mName;
    } process_t;


process_t gTmpActiveProcess;
process_t* gpActiveProcess = &gTmpActiveProcess;


// Symbols from linker. Does not really matter the type of them as the
// address of them is what matters.
extern char         gKernelStart;
extern char         gKernelEnd;


extern void write_cr0(dword_t);
extern void write_cr3(dword_t);
extern dword_t read_cr0();
extern dword_t read_cr3();


/**
 *  RAM size in bytes
 *  TODO: gx 2007-05-15: load at run time
 */
static size_t       gRamSize            = 1024 * 1024 * 8; // 8mb

//static size_t       gKernelStartPage;
//static size_t       gKernelEndPage;


static pointer_t*   gpFreePageStack;
static pointer_t*   gpFreePageStackTop;
static size_t       gTotalPageCount;


/**
 *  Pointer to page directory.
 *
 *  Don't really know if we actually need this (gx)
 */
static pointer_t*  gpPageDirectory;



pointer_t KERNEL_CALL
mm_page_to_pointer(size_t page);

bool_t KERNEL_CALL
mm_is_page_free(pointer_t aPage);

pointer_t KERNEL_CALL
mm_containing_page(const pointer_t aPointer);

void KERNEL_CALL
mm_install_paging();

pointer_t* KERNEL_CALL
mm_alloc_page_table(mm_access_t aAccess);

pointer_t* KERNEL_CALL
mm_alloc_page_directory(mm_access_t aAccess);



void KERNEL_CALL
mm_install()
{
/*
    pointer_t page;
    pointer_t ram_top;
    size_t ram_top_page = MM_KERNEL_START + MM_KERNEL_SIZE;

    gpFreePageStack     = mm_page_to_pointer(ram_top_page);
    gpFreePageStackTop  = gpFreePageStack;
    gPageCount          = MM_RAM_SIZE; // in the future this should be calculated at run-time

    // TODO: 2007-05-14 gx: better calc
    ram_top_page        += gPageCount * sizeof(pointer_t) / MM_PAGE_SIZE + 1;

    for (
            page    = mm_page_to_pointer(gPageCount - 1),
            ram_top = mm_page_to_pointer(ram_top_page);
            page >= ram_top;
            page -= MM_PAGE_SIZE
        )
    {
        mm_free_page(page);
    }
*/

    size_t      used_pages  = 0;
    size_t      stack_size;
    pointer_t   page_addr;          // used in loop
    pointer_t   first_free_addr;

    // assuming that all memory before kernel and kernel itself is used
    if ((dword_t)&gKernelEnd > 1024 * 1024)
    {
        used_pages  = (dword_t)&gKernelEnd >> MM_PAGE_ADDR_SHIFT;
        if ((dword_t)&gKernelEnd & ~MM_PAGE_ADDR_MASK)
            used_pages  += 1;
    }
    else
    {
        used_pages  = 1024 * 1024;
    }

    // putting free pages stack jus after kernel
    gpFreePageStack     = mm_page_to_pointer(used_pages);
    gpFreePageStackTop  = gpFreePageStack;

    // calculating total number of pages that can fit in RAM
    // NOT using last bytes of RAM if its not a full page.
    // emm.. this is a geeky way to write (gRamSize / MM_PAGE_SIZE) but
    // should be a little faster then that. Of course it's worth as this code
    // is executed only once when kernel starts but... let it be... ;-)
    gTotalPageCount = gRamSize >> MM_PAGE_ADDR_SHIFT;

    // calculating space needed for free pages stack
    stack_size      = gTotalPageCount * sizeof(pointer_t);
    used_pages      += stack_size >> MM_PAGE_ADDR_SHIFT;
    if (stack_size & ~MM_PAGE_ADDR_MASK)
        used_pages  += 1;

    // should we optimize this loop? (1GB is 262144 pages)
    first_free_addr = mm_page_to_pointer(used_pages);
    for (
            page_addr   = mm_page_to_pointer(gTotalPageCount - 1);
            page_addr >= first_free_addr;
            page_addr -= MM_PAGE_SIZE
        )
    {
        mm_free_page(page_addr);
    }
    // done with memory management

    //mm_load_kernel_process();

    //mm_install_paging(used_pages);
}


void KERNEL_CALL
mm_load_kernel_process(process_t& arProcess)
{
    arProcess.mMem.mCode    = &gKernelStart;
    arProcess.mMem.mData    = 0;
    arProcess.mMem.mStack   = 0;

    if ((dword_t)&gKernelEnd > 1024 * 1024)
    {
        arProcess.mMem.mHeap    = &gKernelEnd;
    }
    else
    {
        arProcess.mMem.mHeap    = 1024 * 1024;
    }

    arProcess.mMem.mHeapEnd = arProcess.mMem.mHeap;

    arProcess.mMem.mPageDir = mm_page_dir(ACC_SUPER | ACC_RW);

}


void KERNEL_CALL
mm_load_process()
{

}


void KERNEL_CALL
mm_install_paging(size_t aPagesPresent)
{
    size_t  dir;
    size_t  entry;
    size_t  page            = 0;
    size_t  present_pages   = 183;

    //page_table_t*   pPageTable;
    pointer_t*  pPageTable;

    // assuming that page directory is allocated immediately after
    // already used memory
    gpPageDirectory = mm_alloc_page_directory(ACC_SUPER | ACC_RW);
    present_pages   += 1;

    for (
            dir = 0;
            page < present_pages;
            ++dir
        )
    {
        // assuming that page table is allocated immediately after
        // already used memory
        pPageTable      = mm_alloc_page_table(ACC_SUPER | ACC_RW);
        present_pages   += 1;
        // filling page table entries
        for (
                entry = 0;
                page < present_pages && entry < MM_PAGE_TABLE_SIZE;
                ++entry, ++page
            )
        {
            pPageTable[entry]    = (pointer_t)(
                    ((dword_t)mm_page_to_pointer(page)) | ENTRY_PRESENT | ACC_SUPER | ACC_RW
                );

        }

        // adding page table to page directory
        gpPageDirectory[dir]  = (pointer_t)(
                (dword_t)pPageTable | ENTRY_PRESENT | ACC_SUPER | ACC_RW
            );
    }
//printf("page dir at %x %x %x\n", (dword_t)gpPageDirectory, gpPageDirectory[0],
//        ((pointer_t*)((dword_t)gpPageDirectory[0] & 0xFFFFFF00))[0]); kernel_panic();

    /* write_cr3, read_cr3, write_cr0, and read_cr0 all come from the assembly functions */
    write_cr3((dword_t)gpPageDirectory);   // put that page directory address into CR3
    write_cr0(read_cr0() | 0x80000000);     // set the paging bit in CR0 to 1
}


/**
 *  Frees specified page.
 *
 *  Currently if specified memory address is not valid page address page that
 *  containts specified address is freed.
 *
 *  @param  aPage   Pointer of page to be freed.
 */
void KERNEL_CALL
mm_free_page(const pointer_t aPage)
{
    // NOTE: gx 2007-05-13: should memory be freed if page address is not valid?
    pointer_t page  = mm_containing_page(aPage);
    if (mm_is_page_free(page))
        return;

    *gpFreePageStackTop = page;
    gpFreePageStackTop++;

}


/**
 *  Allocates a page of memory.
 *
 *  @return Pointer to memory page.
 */
pointer_t KERNEL_CALL
mm_alloc_page()
{
    if (gpFreePageStackTop == gpFreePageStack)
        return NULL;

    gpFreePageStackTop--;
    return *gpFreePageStackTop;
}


pointer_t* KERNEL_CALL
mm_alloc_page_table(mm_access_t aAccess)
{
    size_t          flags       = aAccess & ACC_MASK;
    pointer_t*      pPageTable  = (pointer_t*)mm_alloc_page();
    size_t          i;
    for (i = 0; i < MM_PAGE_TABLE_SIZE; ++i)
    {
        pPageTable[i]   = (pointer_t)flags;
    }
    return pPageTable;
}


pointer_t* KERNEL_CALL
mm_alloc_page_directory(mm_access_t aAccess)
{
    size_t          flags       = aAccess & ACC_MASK;
    pointer_t*      pPageDir    = (pointer_t*)mm_alloc_page();
    size_t          i;
    for (i = 0; i < MM_PAGE_DIR_SIZE; ++i)
    {
        pPageDir[i] = (pointer_t)flags;
    }
    return pPageDir;
}


/**
 *  Returns number of free pages.
 *
 *  @return Number of free pages.
 */
size_t KERNEL_CALL
mm_get_free_pages()
{
    return gpFreePageStackTop - gpFreePageStack;
}


void KERNEL_CALL
mm_print_info()
{
    size_t  free_pages  = mm_get_free_pages();

    printf("Memory manager\n");
    printf("\taddress:  \t%x\n", gpFreePageStack);
    printf("\tsize:     \t%d bytes\n", gTotalPageCount * sizeof(pointer_t));
    printf("\tfree:     \t%d pages (%d bytes)\n", free_pages,
            free_pages * MM_PAGE_SIZE);
}


/**
 *  Converts page number to page address in memory.
 *
 *  @param  aPage   Page number.
 *  @return Pointer to specified page.
 */
pointer_t KERNEL_CALL
mm_page_to_pointer(size_t aPage)
{
    return (pointer_t)(aPage << MM_PAGE_ADDR_SHIFT);
}


/**
 *  Returns pointer to page containing specified pointer.
 *
 *  @param  aPointer    Pointer in memory.
 *  @return Pointer to page containing specified pointer.
 */
pointer_t KERNEL_CALL
mm_containing_page(const pointer_t aPointer)
{
    return (pointer_t)((size_t)aPointer & MM_PAGE_ADDR_MASK);
}


/**
 *  Checks if page is free.
 *
 *  @param  aPage Pointer to page.
 *  @return TRUE if page is free, FALSE otherwise.
 */
bool_t KERNEL_CALL
mm_is_page_free(const pointer_t aPage)
{
    // TODO: gx 2007-05-13: optimize this! (may be using bitmap of free pages)
    pointer_t* page;
    for (page = gpFreePageStackTop - 1; page >= gpFreePageStack; --page)
        if (aPage == *page)
            return TRUE;
    return FALSE;
}
