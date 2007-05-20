; kernel virtual memory base.
; !!! This value is hardcoded and must be synchronized with linker script base value !!!

    KERNEL_BASE equ 0x80000000;

; page directory 2GB table offset

    PAGE_DIRECTORY_OFFSET       equ KERNEL_BASE / 0x100000

; function aliases ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    F_TELETYPE                  equ 0Eh

; interrupt (service) aliases
    S_VIDEO                     equ 10h

; colors
    CL_GRAY                     equ 07h

; sizes
    STACK_SIZE                  equ 8192    ; stack size in bytes
    PAGE_SIZE                   equ 1000h   ; page size in bytes
    NUM_PAGE_ENTRIES            equ 1024
    NUM_PAGE_TABLES             equ 2

; flags
    PAGE_RW_PRESENT             equ 3
    PAGING_BIT                  equ 80000000h

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; EOF
