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

extern _os_main

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Main                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; this jmp is needed for integrity test
    jmp _start
_start:

    cli		        ; Clear or disable interrupts
    lgdt[gdtr]	    ; Load GDT
    mov eax,cr0	    ; The lsb of cr0 is the protected mode bit
    or al,0x01	    ; Set protected mode bit
    mov cr0,eax	    ; Mov modified word to the control register

    jmp codesel:go_pm

[bits 32]

go_pm:
    mov ax, datasel
    mov ds, ax	         ; Initialise ds & es to data segment
    mov fs, ax
    mov ss, ax

    mov es, ax
    mov ax, videosel     ; Initialise gs to video memory
    mov gs, ax

    mov esp, _sys_stack

    ; ROCK IT MAN!!!!

    ; It is not flat mode
    mov word [gs:0x200], 'R'
    mov word [gs:0x201], 0x09
    mov word [gs:0x202], 'O'
    mov word [gs:0x203], 0x0A
    mov word [gs:0x204], 'C'
    mov word [gs:0x205], 0x0B
    mov word [gs:0x206], 'K'
    mov word [gs:0x207], 0x0C
    mov word [gs:0x208], 'E'
    mov word [gs:0x209], 0x0D
    mov word [gs:0x20A], 'T'
    mov word [gs:0x20B], 0x0E
    mov word [gs:0x20C], 'M'
    mov word [gs:0x20D], 0x0F
    mov word [gs:0x20E], 'A'
    mov word [gs:0x20F], 0x05
    mov word [gs:0x210], 'N'
    mov word [gs:0x211], 0x06

    ; Let's put this symbol in flat mode
    mov word [ds:0x0B8212], '!'
    mov word [ds:0x0B8213], 0x07

    call    _os_main

    spin : jmp spin       ; Loop

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Data                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; i think there is no diference if bits 32 or bits 16 written here
bits 16

; defining this as .data causes tripple fault (somehow related to LD putting it to separate page)
; section .data

; main pointer to gdt
gdtr :
    dw gdt_end-gdt-1   ; Length of the gdt
    dd gdt		       ; physical address of gdt
; the mighty gdt itself
gdt
nullsel equ $-gdt      ; $->current location,so nullsel = 0h
gdt0 		           ; Null descriptor,as per convention gdt0 is 0
   dd 0		           ; Each gdt entry is 8 bytes, so at 08h it is CS
   dd 0                ; In all the segment descriptor is 64 bits
codesel equ $-gdt	   ; This is 8h,ie 2nd descriptor in gdt
code_gdt		       ; Code descriptor 4Gb flat segment at 0000:0000h
   dw 0x0ffff	       ; Limit 4Gb  bits 0-15 of segment descriptor
   dw 0x0000	       ; Base 0h bits 16-31 of segment descriptor (sd)
   db 0x00             ; Base addr of seg 16-23 of 32bit addr,32-39 of sd
   db 0x09a	           ; P,DPL(2),S,TYPE(3),A->Present bit 1,Descriptor
                       ; privilege level 0-3,Segment descriptor 1 ie code
                       ; or data seg descriptor,Type of seg,Accessed bit
   db 0x0cf	           ; Upper 4 bits G,D,0,AVL ->1 segment len is page
                       ; granular, 1 default operation size is 32bit seg
                       ; AVL : Available field for user or OS
                       ; Lower nibble bits 16-19 of segment limit
   db 0x00	           ; Base addr of seg 24-31 of 32bit addr,56-63 of sd
datasel equ $-gdt	   ; ie 10h, beginning of next 8 bytes for data sd
data_gdt		       ; Data descriptor 4Gb flat seg at 0000:0000h
   dw 0x0ffff	       ; Limit 4Gb
   dw 0x0000	       ; Base 0000:0000h
   db 0x00	           ; Descriptor format same as above
   db 0x092
   db 0x0cf
   db 0x00
videosel equ $-gdt     ; ie 18h,next gdt entry
   dw 3999	           ; Limit 80*25*2-1
   dw 0x8000	       ; Base 0xb8000
   db 0x0b
   db 0x92	           ; present,ring 0,data,expand-up,writable
   db 0x00	           ; byte granularity 16 bit
   db 0x00
gdt_end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; EOF

section .bss
    resb 8192               ; This reserves 8KBytes of memory here
_sys_stack:
