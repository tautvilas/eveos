#include "memmgr.h"
#include "stdio.h"

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
#define MM_KERNEL_SIZE      8


static pointer_t*   gpFreePageStack;
static pointer_t*   gpFreePageStackTop;
static size_t       gPageCount;



pointer_t KERNEL_CALL
mm_page_to_pointer(size_t page);

bool_t KERNEL_CALL
mm_is_page_free(pointer_t aPage);

pointer_t KERNEL_CALL
mm_containing_page(const pointer_t aPointer);



void KERNEL_CALL
mm_install()
{
    pointer_t page;
    pointer_t ram_top;
    size_t ram_top_page = MM_KERNEL_START + MM_KERNEL_SIZE;
    
    gpFreePageStack     = mm_page_to_pointer(ram_top_page);
    gpFreePageStackTop  = gpFreePageStack;
    gPageCount          = MM_RAM_SIZE; // in the future this may be calculated at run-time
    
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
        
    gpFreePageStackTop++;
    *gpFreePageStackTop = page;
    
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
        
    return *gpFreePageStackTop--;
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
    printf("\tsize:     \t%d bytes\n", gPageCount * sizeof(pointer_t));
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
        if (aPage == page)
            return TRUE;
    return FALSE;
}
