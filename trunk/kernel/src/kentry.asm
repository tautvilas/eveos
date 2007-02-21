;;;;;;;;;;;;;;;;;;;;;;
; Xos Kernel 1.0     ;
; Author: zvitruolis ;
;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Init                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include "aliases.asm"
; Time for P-mode!
[bits 32]
; Entry symbol for linker
global start
; C main fuction
extern _os_main
extern _gp
global _gdt_flush

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Main                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; this jmp is needed for integrity test
    jmp start
start:
; initialize the system stack
    mov esp, _sys_stack
    jmp continue

mboot:
    ; multiboot headers for GRUB goes here (if needed)

continue:

    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x00:fflush2
fflush2:

    ;;;;;;;;;;;;;;;; DEBUG
    mov al, '!'
    mov ah, TELETYPE_F
    mov bh, 0
    mov bl, GRAY_CL
    int VIDEO_S
    ;;;;;;;;;;;;;;;; EO DEBUG

    ;mov ax, 0
    ;mov fs, ax
    ;mov word [fs:0B8000h], 0x094B

    call _os_main

    jmp $ ; forever loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Proc                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_gdt_flush:
    ;;;;;;;;;;;;;;;; DEBUG
    mov al, 'G'
    mov ah, TELETYPE_F
    mov bh, 0
    mov bl, GRAY_CL
    int VIDEO_S
    ;;;;;;;;;;;;;;;; EO DEBUG

    lgdt [_gp]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2
flush2:
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Data                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Bss                                                   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; 8kb stack
SECTION .bss
    resb STACK_SIZE
_sys_stack:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; EOF
