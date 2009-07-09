#include <mem.h>
#include <mem_physical.h>
#include <algorithms.h>

/*
Directory entries and page table entries share a common format:

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
                
Note: "PRESENT" is actually checked first. If it's not present, 
the entire remainder of the PTE (or directory entry) is available. 
*/

extern "C" Byte gKernelPageDir;
extern "C" void write_cr3(Addr addr);

namespace Mem {


namespace {

    using Generic::divUp;
    using Generic::fill;

    enum
    { 
        PAGE_SIZE   = Physical::PAGE_SIZE, 
        TABLE_SIZE  = PAGE_SIZE / sizeof(Addr),
        DIR_SIZE    = PAGE_SIZE / sizeof(Addr),
        SYS_RW      = 3,
    };
    
    static Addr mAddr       = 0;
    static Addr mUsedEnd    = 0;

    void flush()
    {
        write_cr3(mAddr);
    }

    Addr* tblMem(Index i)
    {
        return addr_cast<Addr*>(-PAGE_SIZE * TABLE_SIZE + PAGE_SIZE * i);
    }

    Addr* tblNew(Index i, Addr tbl)
    {
        addr_cast<Addr*>(-PAGE_SIZE)[i]  = tbl | SYS_RW;
        flush();
        return tblMem(i);
    }
    
    Index pageTbl(Index p)
    {
        return p / TABLE_SIZE; 
    }

}


void 
init()
{
    Physical::init();
    
    mAddr       = addr(&gKernelPageDir) - Kernel::BASE;
    mUsedEnd    = Physical::used() + Kernel::BASE;
    
    // zero-filling unused entries preinitialized in kentry.asm
    Addr*   tbl = tblMem(DIR_SIZE / 2);
    fill(tbl + divUp(Physical::used(), PAGE_SIZE), tbl + TABLE_SIZE, 0);
    flush();
}


Maybe<void*> 
grow(Size size)
{
    // :TODO: 2009-05-02 gx: unallocate memory when returning None()
    
    Index   newPage     = divUp(mUsedEnd, PAGE_SIZE);
    Size    endPage     = divUp(mUsedEnd + size, PAGE_SIZE);   
    Addr*   firstTable  = tblMem(pageTbl(newPage));
    Addr*   tbl         = firstTable;
    
    for (; newPage != endPage; ++newPage)
    {
        Index   p   = newPage % TABLE_SIZE;
        
        if (0 == p)
        {
            if (Maybe<Addr> page = Physical::alloc())
                tbl = tblNew(newPage / TABLE_SIZE, page.value());
            else
                return None();
        }
        
        if (Maybe<Addr> page = Physical::alloc())
            tbl[p]  = page.value() | SYS_RW;
        else
            return None();
            
    }   
    
    // zero-filling remaining new page table entries 
    if (tbl != firstTable) 
        fill(tbl + endPage % TABLE_SIZE, tbl + TABLE_SIZE, 0);
    
    flush();
    
    void*   addr        = addr_cast<void*>(mUsedEnd);
    mUsedEnd            += size;            
    return addr;
}


Size 
used()
{
    return mUsedEnd - Kernel::BASE;
}

}

