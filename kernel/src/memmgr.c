#include "memmgr.h"
#include "stdio.h"
#include "idt.h"
#include "mem.h"

/**
 *  Mask to zero-out unused bits from address
 */
#define MM_PAGE_ADDR_MASK   0xFFFFF000

/**
 *  Number of bits to right-shift page number to get page address
 */
#define MM_PAGE_ADDR_SHIFT  12


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


/**
 *  Bit in CR0 to enable paging
 */
#define MM_ENABLE_PAGING    0x80000000


#define MM_PAGE_TBL_SIZE    1024
#define MM_PAGE_DIR_SIZE    1024


#define MM_SYSTEM_LO_MEM    MEGABYTE


typedef dword_t             mm_access_t;
typedef pointer_t*          mm_page_tbl_t;
typedef mm_page_tbl_t*      mm_page_dir_t;

/*
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
*/


// Symbols from linker. Does not really matter the type of them as the
// address of them is what matters.
extern void         gKernelStart;
extern void         gKernelEnd;
extern void         gKernelBase;


extern void write_cr0(dword_t);
extern void write_cr3(dword_t);
extern dword_t read_cr0();
extern dword_t read_cr3();


/**
 *  RAM size in bytes
 *  TODO: gx 2007-05-15: load at run time
 */
static size_t       gsRamSize            = MEGABYTE * 8; // 8mb

//static size_t       gKernelStartPage;
//static size_t       gKernelEndPage;


static pointer_t*   gspFreePageStack;
static pointer_t*   gspFreePageStackTop;
static size_t       gsTotalPageCount;


/**
 *  Page directory.
 *
 *  Don't really know if we actually need this (gx)
 */
static mm_page_dir_t    gpPageDirectory;
static size_t           gsKernelVmSize;



pointer_t KERNEL_CALL
mm_page_to_pointer(size_t page);

bool_t KERNEL_CALL
mm_is_page_free(pointer_t aPage);

pointer_t KERNEL_CALL
mm_containing_page(const pointer_t aPointer);

size_t KERNEL_CALL
mm_install_paging(size_t aPagesPresent);

pointer_t KERNEL_CALL
mm_alloc_page();

void KERNEL_CALL
mm_free_page(const pointer_t aPage);



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
    dword_t     kernel_end  = (dword_t)&gKernelEnd - (dword_t)&gKernelBase;

    //printf("kernel end: %x\n", kernel_end);

    // assuming that all memory before kernel and kernel itself is used
    if (kernel_end > MM_SYSTEM_LO_MEM)
    {
        used_pages  = kernel_end >> MM_PAGE_ADDR_SHIFT;
        if (kernel_end & ~MM_PAGE_ADDR_MASK)
            used_pages  += 1;
    }
    else
    {
        used_pages  = MM_SYSTEM_LO_MEM / MM_PAGE_SIZE;
    }

    // putting free pages stack jus after kernel
    gspFreePageStack    = (pointer_t)(
            (size_t)mm_page_to_pointer(used_pages) + (size_t)&gKernelBase
        );
    printf("stack top: %x\n", gspFreePageStack);
    gspFreePageStackTop = gspFreePageStack;

    // calculating total number of pages that can fit in RAM
    // NOT using last bytes of RAM if its not a full page.
    // emm.. this is a geeky way to write (gsRamSize / MM_PAGE_SIZE) but
    // should be a little faster then that. Of course it's worth nothing
    // as this code is executed only once when kernel starts but...
    // let it be... ;-)
    gsTotalPageCount    = gsRamSize >> MM_PAGE_ADDR_SHIFT;

    // calculating space needed for free pages stack
    stack_size      = gsTotalPageCount * sizeof(pointer_t);
    //used_pages      += stack_size >> MM_PAGE_ADDR_SHIFT;
    used_pages      += stack_size / MM_PAGE_SIZE;
    //if (stack_size & ~MM_PAGE_ADDR_MASK)
    if (stack_size % MM_PAGE_SIZE)
        used_pages  += 1;

    // should we optimize this loop? (1GB is 262144 pages)
    first_free_addr = mm_page_to_pointer(used_pages);
    for (
            page_addr   = mm_page_to_pointer(gsTotalPageCount - 1);
            page_addr >= first_free_addr;
            page_addr -= MM_PAGE_SIZE
        )
    {
        mm_free_page(page_addr);
    }
    // done with memory management

    //mm_load_kernel_process();

    used_pages      = mm_install_paging(used_pages);
    gsKernelVmSize  = (size_t)&gKernelBase + used_pages * MM_PAGE_SIZE;
}


/*void KERNEL_CALL
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

} */


size_t KERNEL_CALL
mm_install_paging(size_t aPagesPresent)
{
    size_t  dir;
    size_t  entry;
    size_t  page            = 0;
    size_t  pages_present   = aPagesPresent;

    const dword_t ENTRY_FLAGS   = ACC_SUPER | ACC_RW | ENTRY_PRESENT;

    //page_table_t*   pPageTable;
    mm_page_tbl_t   page_table;

    // assuming that page directory is allocated immediately after
    // already used memory
    gpPageDirectory = (mm_page_dir_t)memsetd(mm_alloc_page(), 0, MM_PAGE_DIR_SIZE);
    pages_present   += 1;

    for (
            dir = (dword_t)&gKernelBase / MEGABYTE / 4;
            page < pages_present;
            ++dir
        )
    {
        // assuming that page table is allocated immediately after
        // already used memory
        page_table      = mm_alloc_page();
        pages_present   += 1;

        // filling page table entries
        for (
                entry = 0;
                page < pages_present && entry < MM_PAGE_TBL_SIZE;
                ++entry, ++page
            )
        {
            page_table[entry]    = (pointer_t)(
                    (dword_t)mm_page_to_pointer(page) | ENTRY_FLAGS
                );

        }

        for (; entry < MM_PAGE_TBL_SIZE; ++entry)
        {
            page_table[entry]   = NULL;
        }

        // adding page table to page directory
        gpPageDirectory[dir]  = (pointer_t)(
                (dword_t)page_table | ENTRY_FLAGS
            );
    }

    gpPageDirectory[MM_PAGE_DIR_SIZE - 1]   = (pointer_t)(
            (dword_t)gpPageDirectory | ENTRY_FLAGS
        );


    write_cr3((dword_t)gpPageDirectory);   // put that page directory address into CR3
    gpPageDirectory = (pointer_t)((dword_t)gpPageDirectory + (dword_t)&gKernelBase);

    return pages_present;
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

    *gspFreePageStackTop = page;
    gspFreePageStackTop++;

}


/**
 *  Allocates a page of memory.
 *
 *  @return Pointer to memory page.
 */
pointer_t KERNEL_CALL
mm_alloc_page()
{
    if (gspFreePageStackTop == gspFreePageStack)
        return NULL;

    gspFreePageStackTop--;
    return *gspFreePageStackTop;
}


mm_page_dir_t KERNEL_CALL
mm_page_dir_addr()
{
    // last entry in page directory points to it self
    // this means we can use highest page below 4gb mark to access it
    return (mm_page_dir_t)((size_t)-1 - MM_PAGE_SIZE + 1);
}


mm_page_dir_t KERNEL_CALL
mm_page_dir_phys_addr()
{
    pointer_t*  pDirAddr    = (pointer_t*)((size_t)-1 - sizeof(pointer_t) + 1);
    return (mm_page_dir_t)((dword_t)*pDirAddr & MM_PAGE_ADDR_MASK);
}


mm_page_tbl_t KERNEL_CALL
mm_page_tbl_addr(size_t aIndex)
{
    return (mm_page_tbl_t)(
            (size_t)-1
            - MM_PAGE_TBL_SIZE * MM_PAGE_SIZE + 1
            + aIndex * MM_PAGE_SIZE
        );
}


mm_page_tbl_t KERNEL_CALL
mm_page_tbl_phys_addr(size_t aIndex)
{
    mm_page_dir_t   pPageDir    = mm_page_dir_addr();
    return (mm_page_tbl_t)((dword_t)pPageDir[aIndex] & MM_PAGE_ADDR_MASK);
}


size_t KERNEL_CALL
mm_paging_free_pages(size_t aIndex, size_t aCount)
{
    // frees backwards e.g. inf aIndex is 5 and aCount is 2 then pages
    // 5, 4 are freed
    size_t  tbl_i;
    size_t  page_i;
    size_t  count;
    mm_page_dir_t   pPageDir;
    mm_page_tbl_t   pTbl;

    tbl_i   = aIndex / MM_PAGE_TBL_SIZE;
    if (0 == aIndex % MM_PAGE_TBL_SIZE)
        tbl_i--;

    page_i  = aIndex % MM_PAGE_TBL_SIZE;
    if (0 == page_i)
        page_i  = MM_PAGE_TBL_SIZE - 1;

    pPageDir    = mm_page_dir_addr();
    pTbl        = mm_page_tbl_addr(tbl_i); //(mm_page_tbl_t)((dword_t)pPageDir[tbl_i] & MM_PAGE_ADDR_MASK);

    for (count = 0; count < aCount; ++count)
    {
        mm_free_page((pointer_t)((dword_t)pTbl[page_i] & MM_PAGE_ADDR_MASK));

        if (0 == page_i)
        {
            mm_free_page(mm_page_tbl_phys_addr(tbl_i));
            pPageDir[tbl_i] = NULL;
            tbl_i--;
            page_i  = MM_PAGE_TBL_SIZE - 1;
            pTbl    = mm_page_tbl_addr(tbl_i); //(mm_page_tbl_t)((dword_t)pPageDir[tbl_i] & MM_PAGE_ADDR_MASK);
        }
        else
        {
            pTbl[page_i]    = NULL;
            page_i--;
        }
    }

    write_cr3((dword_t)mm_page_dir_phys_addr());

    return count;
}


size_t KERNEL_CALL
mm_paging_alloc_pages(size_t aIndex, size_t aCount, mm_access_t aAccess)
{
    size_t  tbl_c;
    size_t  page_c;
    size_t  count;
    mm_page_dir_t   pPageDir;
    mm_page_tbl_t   pTbl;

    const dword_t   ENTRY_FLAGS = (aAccess & ACC_MASK) | ENTRY_PRESENT;

    tbl_c   = aIndex / MM_PAGE_TBL_SIZE;
    if (aIndex % MM_PAGE_TBL_SIZE)
        tbl_c++;

    page_c  = aIndex % MM_PAGE_TBL_SIZE;

    pTbl        = mm_page_tbl_addr(tbl_c - 1);
    pPageDir    = mm_page_dir_addr();

    for (count = 0; count < aCount; ++count)
    {
        pointer_t       pPage;

        pPage   = mm_alloc_page();
        if (NULL == pPage)
        {
            // :TODO: gx 2007-05-23: rollback
            return count;
        }

        if (0 == page_c)
        {
            // creating new page table
            mm_page_tbl_t pNewTbl   = mm_alloc_page();
            if (NULL == pNewTbl)
            {
                // :TODO: gx 2007-05-23: rollback
                return count;
            }
            pPageDir[tbl_c] = (mm_page_tbl_t)((dword_t)pNewTbl | ENTRY_FLAGS);
            pTbl            = mm_page_tbl_addr(tbl_c);
            tbl_c++;
        }

        pTbl[page_c]    = (pointer_t)((dword_t)pPage | ENTRY_FLAGS);
        // TODO: gx 2007-05-24: would by nice to fill new memory with zeros but
        //      we can not access it beacause page dir is not up to date
        page_c          = (page_c + 1) % MM_PAGE_TBL_SIZE;
    }

    write_cr3((dword_t)mm_page_dir_phys_addr());

    return count;
}


int KERNEL_CALL
brk(pointer_t pEnd)
{
    // :TODO: gx 2007-05-24: adapt to work not only for kernel memory
    pointer_t   pCurrEnd    = (pointer_t)gsKernelVmSize;
    pointer_t   pCurrPageEnd;
    pointer_t   pCurrPageStart;

    // if address exceeds allowed range, return failure
    // :TODO: gx 2007-05-23:
    //      1) get values from somewhere reasonable
    //      2) set errno on error
    if ((pEnd < (pointer_t)&gKernelEnd) || (pEnd > (pointer_t)(3U * GIGABYTE)))
        return -1;

    // callculating the start and end addresses of the last actually allocated
    // memory page
    pCurrPageStart  = (pointer_t)((dword_t)pCurrEnd & MM_PAGE_ADDR_MASK);
    pCurrPageEnd    = pCurrPageStart;
    if ((size_t)pCurrEnd & ~MM_PAGE_ADDR_MASK)
        pCurrPageEnd    += MM_PAGE_SIZE;
    else
        pCurrPageStart  -= MM_PAGE_SIZE;


    // check if pEnd fits in current page
    if (pEnd > pCurrPageStart && pEnd <= pCurrPageEnd)
    {
        gsKernelVmSize  = (size_t)pEnd;
        return 0;
    }

    if (pEnd > pCurrPageEnd)
    {
        // allocate needed pages
        size_t  allocated;
        size_t  delta   = pEnd - pCurrPageEnd;
        size_t  count   = delta / MM_PAGE_SIZE;
        if (delta % MM_PAGE_SIZE)
            count++;
        allocated   = mm_paging_alloc_pages(
                (size_t)pCurrPageEnd / MM_PAGE_SIZE,
                count,
                ACC_RW | ACC_SUPER
            );
        if (allocated < count)
            return -1;
    }
    else if (pEnd <= pCurrPageStart)
    {
        mm_paging_free_pages(
                (size_t)pCurrPageStart / MM_PAGE_SIZE,
                (pCurrPageStart - pEnd) / MM_PAGE_SIZE + 1
            );
    }

    gsKernelVmSize  = (size_t)pEnd;

    return 0;
}


pointer_t KERNEL_CALL
sbrk(int aIncrement)
{
    pointer_t   addr    = (pointer_t)gsKernelVmSize;

    if (aIncrement == 0)
        return addr;

    if (0 == brk((pointer_t)(gsKernelVmSize + aIncrement)))
        return addr;

    return NULL;
}

pointer_t KERNEL_CALL
sbrk_old(size_t aBytes)
{
    size_t      page_c;
    size_t      tbl_c;
    size_t      allocated;
    pointer_t   addr;
    size_t      new_size    = gsKernelVmSize + aBytes;
    pointer_t   page_addr;

    if (new_size < gsKernelVmSize)
        return (pointer_t)-1;

    // :TODO: gx 2007-05-23: check for max size

    addr    = (pointer_t)gsKernelVmSize;

    page_c  = gsKernelVmSize / MM_PAGE_SIZE;
    if (gsKernelVmSize % MM_PAGE_SIZE)
        page_c--;
    allocated   = page_c * MM_PAGE_SIZE;
    if (new_size <= allocated)
    {
        gsKernelVmSize  = new_size;
        return addr;
    }

    tbl_c   = page_c / MM_PAGE_TBL_SIZE;
    if (page_c % MM_PAGE_TBL_SIZE)
        tbl_c++;
    page_c    = page_c % MM_PAGE_TBL_SIZE;
    while (allocated < new_size)
    {
        mm_page_tbl_t   tbl;

        page_addr   = mm_alloc_page();
        if (NULL == page_addr)
        {
            // :TODO: gx 2007-05-23: rollback
            return (pointer_t)-1;
        }

        if (0 == page_c)
        {
            tbl = mm_alloc_page();
            if (NULL == tbl)
            {
                // :TODO: gx 2007-05-23: rollback
                return (pointer_t)-1;
            }
            gpPageDirectory[tbl_c]    = (pointer_t)(
                    (dword_t)tbl | ACC_RW | ACC_SUPER | ENTRY_PRESENT
                );
            write_cr3((dword_t)gpPageDirectory - (dword_t)&gKernelBase);
            tbl_c++;
        }
        else
        {
            tbl = (pointer_t)(
                    (dword_t)gpPageDirectory[tbl_c - 1] & MM_PAGE_ADDR_MASK
                );
        }
        tbl = (pointer_t)((unsigned)(MM_PAGE_DIR_SIZE - 1) * MM_PAGE_SIZE * MM_PAGE_TBL_SIZE
                + (tbl_c - 1) * MM_PAGE_SIZE);

        tbl[page_c] = (pointer_t)(
                (dword_t)page_addr | ACC_RW | ACC_SUPER | ENTRY_PRESENT
            );
        write_cr3((dword_t)gpPageDirectory - (dword_t)&gKernelBase);

        allocated   += MM_PAGE_SIZE;
        page_c      = (page_c + 1) % MM_PAGE_TBL_SIZE;
    }

    gsKernelVmSize  = new_size;

    return addr;
}


/*
pointer_t* KERNEL_CALL
mm_alloc_page_table(mm_access_t aAccess)
{
    size_t          flags       = aAccess & ACC_MASK;
    pointer_t*      pPageTable  = (pointer_t*)mm_alloc_page();
    size_t          i;
    for (i = 0; i < MM_PAGE_TBL_SIZE; ++i)
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
}*/


/**
 *  Returns number of free pages.
 *
 *  @return Number of free pages.
 */
size_t KERNEL_CALL
mm_get_free_pages()
{
    return gspFreePageStackTop - gspFreePageStack;
}


void KERNEL_CALL
mm_print_info()
{
    size_t  free_pages  = mm_get_free_pages();

    printf("Memory manager\n");
    printf("\taddress:  \t%x\n", gspFreePageStack);
    printf("\tsize:     \t%d bytes\n", gsTotalPageCount * sizeof(pointer_t));
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
    for (page = gspFreePageStackTop - 1; page >= gspFreePageStack; --page)
        if (aPage == *page)
            return TRUE;
    return FALSE;
}
