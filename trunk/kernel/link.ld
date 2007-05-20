OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)
ENTRY(_start)                   /* kernel entry point */
_gVmBase    = 0xF00008000       /* virtual address of kernel */
_gPhysBase  = 0x000008000       /* physical address of kernel */

/* this instructs the linker how to lay out sections in the output */
SECTIONS
{
    .text _gVmBase : AT(_gVmBase) {
        _gKernelStart = .;
        code = .;
        *(.text)
            . = ALIGN(4096); /* align every section on a separate page */
    }
    .data : AT(_gVmBase + (data - code))
    {
        data = .;
        /* rodata is a special section made by GCC for read only data
           this section can have several parts like
                rodata.str1.4
                rodata.str1.1
           and maybe other, that is why i wrote .rodata*
         */
        *(.rodata*)
        *(.rdata)
        *(.data)
            . = ALIGN(4096);
    }
    .bss : AT(_gVmBase + (bss - code))
    {
        bss = .;
        *(.bss)
            . = ALIGN(4096);
    }
    /*end = .; /* this marks the end of all sections */
    _gKernelEnd = .;
}

