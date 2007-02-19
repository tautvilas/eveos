; Aliases ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; function aliases
    TELETYPE_F equ 0x0E
    READ_CHARACTER_F equ 0x00
    SET_VIDEO_MODE_F equ 0x00
    READ_SECT_FROM_DRIVE_F equ 0x02
; interrupt (service) aliases
    DISK_S equ 0x13
    VIDEO_S equ 0x10
    KEYBOARD_S equ 0x16
; colors
    GRAY_CL equ 0x07
; memory addresses
    BOOT_SEG equ 0000h
    BOOT_OFF equ 7C00h
    KERNEL_SEG equ 0800h
    KERNEL_OFF equ 0000h
    REBOOT_SEG equ 0040h
    REBOOT_OFF equ 0072h
    REBOOT_JMP_SEG equ 0ffffh
    REBOOT_JMP_OFF equ 0000h
    STACK_SEG equ 07C0h ; the same as 0000:7C00
    STACK_OFF equ 03feh
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
