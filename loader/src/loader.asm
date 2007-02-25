;;;;;;;;;;;;;;;;;;;;;;
; Xos Bootloader 1.0 ;
; Author: zvitruolis ;
;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Init                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include "aliases.asm"
;PC starts booting in real mode
[bits 16]
;Location in mem where bootsector is about to be stored
[org 7C00h]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Main                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Set up the DS
    mov ax, BOOT_SEG
    mov ds, ax
;initialize the stack
    mov ax, STACK_SEG
    mov ss, ax
    mov sp, STACK_OFF ;stack top
;set default video mode 80x25:
    mov ah, SET_VIDEO_MODE_F
    mov al, VIDEO_MODE_80x25
    int VIDEO_S
;Print out OS title
    mov si, titleStr
    call __printStr
;Load kernel from floppy sector 2 to 0800h:0000h - es:bx
    mov si, loadingFromFd0Str
    call __printStr
    call __loadKernel
;kernel integrity check:
    mov ax, KERNEL_SEG
    mov es, ax
    cmp byte [es:KERNEL_OFF], FIRST_KERNEL_BYTE
    je integrity_check_ok
;integrity check error
    mov si, loadKernErrStr
    call __printStr
;wait for any key...
    call __waitForKey
;store magic value at 0040h:0072h:
    mov ax, REBOOT_SEG
    mov ds, ax
    mov word [REBOOT_OFF], COLD_BOOT
;now reboot!
    jmp REBOOT_JMP_SEG:REBOOT_JMP_OFF
integrity_check_ok:
;pass control to kernel:
    jmp KERNEL_SEG:KERNEL_OFF

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Procedures                                            ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Function to print out a string, which address is located in [SI]
__printStr:
    push ax
    push bx
    push si

    mov ah, TELETYPE_F ;Function to display a chacter (teletype)
    mov bh, 0          ;Page number
    mov bl, GRAY_CL    ;Gray text color
.nextchar
    lodsb         ;Loads [SI] into AL and increases SI by one
    cmp al, 0     ;Check for end of string '0'
    jz .return
    int VIDEO_S
    jmp .nextchar ;Go to check next char
.return

    pop si
    pop bx
    pop ax
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;load the kernel at 0800h:0000h
;BIOS passes drive number in dl,
;so it's not changed:

__loadKernel:
    push ax
    push bx
    push cx
    push es

    mov ah, READ_SECT_FROM_DRIVE_F
;al keeps how many sectors to read
    mov al, KERNEL_SIZE
    mov ch, 0 ;cylinder.
    mov cl, 2 ;sector.
    mov dh, 0 ;head.
;dl not changed! - drive number

;es:bx points to receiving
;data buffer:
    mov bx, KERNEL_SEG
    mov es, bx
    mov bx, KERNEL_OFF
;read!
    int DISK_S

    pop es
    pop cx
    pop bx
    pop ax
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

__waitForKey:
    mov ah, READ_CHARACTER_F
    int KEYBOARD_S
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

__debug:
    mov si, debugStr
    call __printStr
ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Data                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

titleStr db          'K-loader started!', 13, 10, 0
debugStr db          '###### debug message ######', 13, 10, 0
loadingFromFd0Str db 'Trying to read kernel from disc sector 2, cylinder 0, head 0...', 13, 10, 0
loadKernErrStr db    'The kernel file is invalid!', 13, 10
               db    'System will now reboot. Pres any key to continue...', 13, 10, 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Ending                                                ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Fill all remaining of 510 bytes with 0
;$-$$ means [start of the instruction - start of the program]
    times 510-($-$$) db 0
;The magic value to comple 512b sector
    dw BOOTSECT_MAGIC_VALUE

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; EOF
