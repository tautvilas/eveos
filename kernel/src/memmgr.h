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


pointer_t KERNEL_CALL
sbrk(int aIncrement);


int KERNEL_CALL
brk(pointer_t pEnd);


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
