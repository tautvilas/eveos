;;;;;;;;;;;;;;;;;;;;;;
; Xos Kernel 1.0     ;
; Author: zvitruolis ;
;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Init                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include "include/aliases.asm"
; Time for P-mode!
[bits 32]
; Entry symbol for linker
global start
; C main fuction
extern main

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
	;mov ax, 0B000h
	;mov es, ax
	;mov word [es:8000h], 0x094B

	;;;;;;;;;;;;;;;; DEBUG
	mov al, '!'
	mov ah, TELETYPE_F
	mov bh, 0
	mov bl, GRAY_CL
	int VIDEO_S
	;;;;;;;;;;;;;;;; EO DEBUG

	call main
	jmp $ ; forever loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Proc                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
