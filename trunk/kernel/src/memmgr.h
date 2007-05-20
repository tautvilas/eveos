#ifndef _MEMMGR_H_
#define _MEMMGR_H_

#include "global.h"

/**
 *  Page size in bytes
 */
#define MM_PAGE_SIZE        4096


/**
 *  Installs memory manager.
 */
void KERNEL_CALL
mm_install();


/**
 *  Allocates a page of memory.
 *
 *  @return Pointer to memory page.
 */
pointer_t KERNEL_CALL
mm_alloc_page();


/**
 *  Frees specified page.
 *
 *  Currently if specified memory address is not valid page address, page that
 *  containts specified address is freed.
 *
 *  @param  aPage   Pointer of page to be freed.
 */
void KERNEL_CALL
mm_free_page(const pointer_t aPage);


/**
 *  Returns number of free pages.
 *
 *  @return Number of free pages.
 */
size_t KERNEL_CALL
mm_get_free_pages();


/**
 *  Prints memory manager info.
 */
void KERNEL_CALL
mm_print_info();


#endif  // _MEMMGR_H_
