;;;;;;;;;;;;;;;;;;;;
; EveOS Kernel 1.0 ;
;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Init                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include "aliases.asm"
[bits 16]
; Entry symbol for linker
[global _start]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Main                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; this jmp is needed for integrity test
    jmp _start
_start:
    xor eax, eax
    mov ax, ds
    shl eax, 4
    add eax, gdt
    mov [gdt_ptr + 2], eax
    lgdt [gdt_ptr]
    mov ax, 0x08
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    jmp 0x10:jump
jump:

[bits 32]
    mov word [0B8000h], 0x094B
    jmp $

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Data                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION .data

gdt:
; NULL descriptor
	dw 0		; limit 15:0
	dw 0		; base 15:0
	db 0		; base 23:16
	db 0		; type
	db 0		; limit 19:16, flags
	db 0		; base 31:24

LINEAR_DATA_SEL	equ	$-gdt
	dw 0FFFFh
	dw 0
	db 0
	db 92h		; present, ring 0, data, expand-up, writable
	db 0CFh		; page-granular (4 gig limit), 32-bit
	db 0

LINEAR_CODE_SEL	equ	$-gdt
	dw 0FFFFh
	dw 0
	db 0
	db 9Ah		; present,ring 0,code,non-conforming,readable
	db 0CFh		; page-granular (4 gig limit), 32-bit
	db 0
gdt_end:

gdt_ptr:
	dw gdt_end - gdt - 1
	dd gdt

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; EOF
