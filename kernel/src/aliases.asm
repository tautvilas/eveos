; function aliases ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    F_TELETYPE equ 0x0E
    F_READ_CHARACTER equ 0x00
    F_SET_VIDEO_MODE equ 0x00
    F_READ_SECT_FROM_DRIVE equ 0x02
; interrupt (service) aliases
    S_DISK equ 0x13
    S_VIDEO equ 0x10
    S_KEYBOARD equ 0x16
; colors
    CL_GRAY equ 0x07
; memory addresses
    SEG_BOOT equ 0000h
    OFF_BOOT equ 7C00h
    SEG_KERNEL equ 0800h
    OFF_KERNEL equ 0000h
    SEG_REBOOT equ 0040h
    OFF_REBOOT equ 0072h
    SEG_REBOOT_JMP equ 0ffffh
    OFF_REBOOT_JMP equ 0000h
    SEG_STACK equ 07C0h ; the same as 0000:7C00
    OFF_STACK equ 03feh
; sizes
    KERNEL_SIZE equ 0x01 ; kernel size in sectors
    STACK_SIZE equ 8192 ; stack size in bytes
; other stuff
    FIRST_KERNEL_BYTE equ 0xE9
    COLD_BOOT equ 0000h
    WARM_BOOT equ 1234h
    BOOTSECT_MAGIC_VALUE equ 55AAh
    VIDEO_MODE_80x25 equ 0x03

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; EOF
