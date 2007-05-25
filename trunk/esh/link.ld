OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)
ENTRY(_start)           /* app entry point */

_gAppStart = 0x00;

_gHeaderSize    = _gHeaderEnd   - _gHeaderStart;
_gCodeSize      = _gCodeEnd     - _gCodeStart;
_gDataSize      = _gDataEnd     - _gDataStart;
_gBssSize       = _gBssEnd      - _gBssStart;

/* this instructs the linker how to lay out sections in the output */
SECTIONS
{
    .header : AT(_gAppStart)
    {
        _gHeaderStart = .;
        *(.header)
        _gHeaderEnd = .;
    }
    .text : AT(_gAppStart + (_gCodeStart - _gHeaderStart))
    {
        _gCodeStart = .;
        *(.text)
        _gCodeEnd = .;
    }
    .data : AT(_gAppStart + (_gDataStart - _gHeaderStart))
    {
        _gDataStart = .;
        /* rodata is a special section made by GCC for read only data
           this section can have several parts like
                rodata.str1.4
                rodata.str1.1
           and maybe other, that is why i wrote .rodata*
         */
        *(.rodata*)
        *(.rdata*)
        *(.data)
        _gDataEnd = .;
    }
    .bss : AT(_gAppStart + (_gBssStart - _gHeaderStart))
    {
        _gBssStart = .;
        *(.bss*)
        *(COMMON)
        _gBssEnd = .;
    }
    /* this marks the end of all sections */
    _gAppEnd = .;

    /* ignore these gcc misc sections */
    /DISCARD/ : {
        *(.comment*)
        *(.note*)
    }
}

