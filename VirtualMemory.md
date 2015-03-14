# Page Translation #

The 80386 transforms a linear address into a physical address. This phase of address transformation implements the basic features needed for page-oriented virtual-memory systems and page-level protection.

The page-translation step is optional. Page translation is in effect only when the PG bit of CR0 is set. This bit is typically set by the operating system during software initialization.



# Page Frame #

A page frame is a 4K-byte unit of contiguous addresses of physical memory. Pages begin onbyte boundaries and are fixed in size.

The page frame address specifies the physical starting address of a page. Because pages are located on 4K boundaries, the low-order 12 bits are always zero. In a page directory, the page frame address is the address of a page table. In a second-level page table, the page frame address is the address of the page frame that contains the desired memory operand.



# Page Tables #

**A page table is simply an array of 32-bit page specifiers.** A page table is itself a page, and therefore contains 4 Kilobytes of memory or at most **1K 32-bit entries**.

**Two levels of tables are used to address a page of memory.** At the _higher level is a page directory_. The page directory addresses up to _1K page tables of the second level_. A page table of the second level addresses up to _1K pages_. All the tables addressed by one page directory, therefore, can address 1M pages (2`^`(20)). Because each page contains 4K bytes 2`^`(12) bytes), the tables of one page directory can span the entire physical address space of the 80386 (2`^`(20) times 2`^`(12) = 2`^`(32)).

**The physical address of the current page directory is stored in the CPU register CR3**, also called the page directory base register (**PDBR**). Memory management software has the option of using one page directory for all tasks, one page directory for each task, or some combination of the two.



# Page-Table Entries #

Entries in either level of page tables have the same format:
```
       31                                  12 11                      0
      +--------------------------------------+-------+---+-+-+---+-+-+-+
      |                                      |       |   | | |   |U|R| |
      |      PAGE FRAME ADDRESS 31..12       | AVAIL |0 0|D|A|0 0|/|/|P|
      |                                      |       |   | | |   |S|W| |
      +--------------------------------------+-------+---+-+-+---+-+-+-+

                P      - PRESENT
                R/W    - READ/WRITE
                U/S    - USER/SUPERVISOR
                A      - ACCESSED
                D      - DIRTY (PAGE TABLE ENTRY ONLY)
                AVAIL  - AVAILABLE FOR SYSTEMS PROGRAMMER USE

                NOTE: 0 INDICATES INTEL RESERVED. DO NOT DEFINE.
```


## Present Bit ##

The Present bit indicates whether a page table entry can be used in address translation.

P=1 indicates that the entry can be used.

P=0 in either level of page tables, the entry is not valid for address translation, and the rest of the entry is available for software use; none of the other bits in the entry is tested by the hardware:
```
       31                                                           1 0
      +--------------------------------------------------------------+-+
      |                                                              | |
      |                            AVAILABLE                         |0|
      |                                                              | |
      +--------------------------------------------------------------+-+
```

If P=0 in either level of page tables when an attempt is made to use a page-table entry for address translation, the processor signals a page exception. In software systems that support paged virtual memory, the page-not-present exception handler can bring the required page into physical memory. The instruction that caused the exception can then be reexecuted.

Note that there is no present bit for the page directory itself. The page directory may be not-present while the associated task is suspended, but the operating system must ensure that the page directory indicated by the CR3 image in the TSS is present in physical memory before the task is dispatched.


## Read/Write Bit ##

  1. R/W=0 -- Read-only access
  1. R/W=1 -- Read/write access

When the processor is executing at supervisor level, all pages are both readable and writable. When the processor is executing at user level, only pages that belong to user level and are marked for read/write access are writable; pages that belong to supervisor level are neither readable nor writable from user level.


## User/Supervisor Bit ##

  1. U/S=0 -- Supervisor level; for the operating system and other systems software and related data.
  1. U/S=1 -- User leve; for applications procedures and data.

The current level (U or S) is related to CPL. If CPL is 0, 1, or 2, the processor is executing at supervisor level. If CPL is 3, the processor is executing at user level.

When the processor is executing at supervisor level, all pages are addressable, but, when the processor is executing at user level, only pages that belong to the user level are addressable.


## Accessed and Dirty Bits ##

These bits provide data about page usage. These bits are **set by the hardware**; however, the processor does not clear any of these bits.

The processor sets the corresponding **accessed** bits in **both levels** of page tables to one **before a read or write** operation to a page.

The processor sets the **dirty** bit in the **second-level** page table to one **before a write** to an address covered by that page table entry. The dirty bit in directory entries is undefined.

An operating system that supports paged virtual memory can use these bits to determine what pages to eliminate from physical memory when the demand for memory exceeds the physical memory available. The operating system is responsible for testing and clearing these bits.


# Page Translation Cache #

For greatest efficiency in address translation, the processor stores the most recently used page-table data in an on-chip cache. Only if the necessary paging information is not in the cache must both levels of page tables be referenced.

The existence of the page-translation cache is invisible to applications programmers but not to systems programmers; **operating-system programmers must flush the cache whenever the page tables are changed.** The page-translation cache can be flushed by either of two methods:

  1. By reloading CR3 with a MOV instruction; for example:
```
  MOV CR3, EAX
```
  1. By performing a task switch to a TSS that has a different CR3 image than the current TSS.



---

References:
  1. [Intel 80386 Programmer's Reference Manual: 5.2 Page Translation](http://www.logix.cz/michal/doc/i386/chp05-02.htm)
  1. [Intel 80386 Programmer's Reference Manual: 6.4 Page-Level Protection](http://www.logix.cz/michal/doc/i386/chp06-04.htm)