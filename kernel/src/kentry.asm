;;;;;;;;;;;;;;;;;;;;
; EveOS Kernel 1.0 ;
;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Init                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bits 16
; Entry symbol for linker
global _start
; OS main C function
extern _os_main

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; text                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION .text

    ; this jmp is needed for integrity test
    jmp _start
_start:

    cli             ; Clear or disable interrupts
    lgdt[gdtr]      ; Load GDT
    mov eax, cr0    ; The lsb of cr0 is the protected mode bit
    or al, 1        ; Set protected mode bit
    mov cr0, eax    ; Mov modified word to the control register
    ; far jump is for setting code segment
    jmp CODE_SEL:go_pm

bits 32

go_pm:
    ; initialise all segments to data selector
    mov ax, DATA_SEL
    mov ds, ax
    mov fs, ax
    mov es, ax
    mov ss, ax
    mov gs, ax

    mov esp, _sys_stack

    ; here is the master call
    call _os_main

    cli
    hlt

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; data                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION .data

; main pointer to gdt
gdtr :
    dw gdt_end-gdt-1   ; Length of the gdt - 1
    dd gdt             ; physical address of gdt
; the mighty gdt itself
gdt
NULL_SEL equ $-gdt     ; $->current location,so nullsel = 0h
gdt0                   ; Null descriptor,as per convention gdt0 is 0
    dd 0               ; Each gdt entry is 8 bytes, so at 08h it is CS
    dd 0               ; In all the segment descriptor is 64 bits
CODE_SEL equ $-gdt     ; This is 8h,ie 2nd descriptor in gdt
code_gd                ; Code descriptor 4Gb flat segment at 0000:0000h
    dw 0xffffb         ; Limit 4Gb  bits 0-15 of segment descriptor
    dw 0x0000          ; Base 0h bits 16-31 of segment descriptor (sd)
    db 0x00            ; Base addr of seg 16-23 of 32bit addr,32-39 of sd
    db 10011010b       ; P,DPL(2),S,TYPE(3),A->Present bit 1,Descriptor
                       ; privilege level 0-3,Segment descriptor 1 ie code
                       ; or data seg descriptor,Type of seg,Accessed bit
    db 11001111b       ; Upper 4 bits G,D,0,AVL ->1 segment len is page
                       ; granular, 1 default operation size is 32bit seg
                       ; AVL : Available field for user or OS
                       ; Lower nibble bits 16-19 of segment limit
    db 0x00            ; Base addr of seg 24-31 of 32bit addr,56-63 of sd
DATA_SEL equ $-gdt     ; ie 10h, beginning of next 8 bytes for data sd
data_gd                ; Data descriptor 4Gb flat seg at 0000:0000h
    dw 0x0ffff         ; Limit 4Gb
    dw 0x0000          ; Base 0000:0000h
    db 0x00            ; Descriptor format same as above
    db 10010010b
    db 11001111b
    db 0x00
gdt_end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BSS                                                   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; system stack
SECTION .bss
    resb 8192          ; This reserves 8KBytes of memory
_sys_stack:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; EOF
