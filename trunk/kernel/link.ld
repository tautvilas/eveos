OUTPUT_FORMAT("binary")
ENTRY(_start)               /* kernel entry point */
phys = 0x00008000;          /* address in memory where kernel is loaded */

/* this instructs the linker how to lay out sections in the output */
SECTIONS
{
    .text phys : AT(phys) {
        code = .;
        *(.text)
            . = ALIGN(4096); /* align every section on a separate page */
    }
    .data : AT(phys + (data - code))
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
    .bss : AT(phys + (bss - code))
    {
        bss = .;
        *(.bss)
            . = ALIGN(4096);
    }
    end = .; /* this marks the end of all sections */
}

