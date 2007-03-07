;;;;;;;;;;;;;;;;;;;;;;
; EveOS Kernel 0.0.1 ;
;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Init                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include "aliases.asm"

bits 16

global _start     ; Entry symbol for linker
global _idt_load

extern _os_main           ; OS main C function
extern _gIdtp             ; Pointer to IDT
extern _exception_handler ; ISRs handler

; ISR references

global _isr0    ; division by zero exception
global _isr1    ; debug exception
global _isr2    ; non maskable interrupt exception
global _isr3    ; breakpoint exception
global _isr4    ; into detected overflow exception
global _isr5    ; out of bounds exception
global _isr6    ; invalid opcode exception
global _isr7    ; no coprocessor exception
global _isr8    ; double fault exception (pushes error code)
global _isr9    ; coprocessor segment overrun exception
global _isr10   ; bad TSS exception (pushes error code)
global _isr11   ; segment not present exception (pushes error code)
global _isr12   ; stack fault exception (pushes error code)
global _isr13   ; general protection fault exception (pushes error code)
global _isr14   ; page fault exception (pushes error code)
global _isr15   ; unknown interrupt exception
global _isr16   ; coprocessor fault exception
global _isr17   ; aligment check exception (486+)
global _isr18   ; machine check exception (pentium/586+)

; reserved exceptions

global _isr19
global _isr20
global _isr21
global _isr22
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Code                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION .text

    ; this jmp is needed for integrity test
    jmp _start
_start:

    mov si, title
    call print_str

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
; Proc                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bits 16

; Function to print out a string, which address is located in [SI]
print_str:
    push ax
    push bx
    push si

    mov ah, F_TELETYPE ; Function to display a chacter (teletype)
    mov bh, 0          ; Page number
    mov bl, CL_GRAY    ; Gray text color
.nextchar
    lodsb         ; Loads [SI] into AL and increases SI by one
    cmp al, 0     ; Check for end of string '0'
    jz .return
    int S_VIDEO
    jmp .nextchar ; Go to check next char
.return

    pop si
    pop bx
    pop ax
ret

bits 32

; Load the interrupt descriptor table
_idt_load:
    lidt [_gIdtp]
    ret

; ISRS

_isr0:
    cli
    push byte 0    ; A normal ISR stub that pops a dummy error code to keep a
                   ; uniform stack frame
    push byte 0
    jmp isr_common

_isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common

_isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common

_isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common

_isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common

_isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common

_isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common

_isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common

_isr8:
    cli
    push byte 8
    jmp isr_common

_isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common

_isr10:
    cli
    push byte 10
    jmp isr_common

_isr11:
    cli
    push byte 11
    jmp isr_common

_isr12:
    cli
    push byte 12
    jmp isr_common

_isr13:
    cli
    push byte 13
    jmp isr_common

_isr14:
    cli
    push byte 14
    jmp isr_common

_isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common

_isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common

_isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common

_isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common

_isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common

_isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common

_isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common

_isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common

_isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common

_isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common

_isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common

_isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common

_isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common

_isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common

_isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common

_isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common

_isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common

isr_common:
    pusha
    push ds
    push fs
    push es
    push gs
    mov ax, DATA_SEL
    mov ds, ax
    mov fs, ax
    mov es, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, _exception_handler
    call eax    ; a special call, preserves 'eip' register
    pop eax
    pop gs
    pop es
    pop fs
    pop ds
    popa
    add esp, 8  ; cleans up pushed error code and ISR number
    iret        ; pops cs, eip, eflags, ss and esp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Data                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION .data

title          db 13, 10, "EveOS kernel v0.0.1 is starting, please fasten your seatbelts", 13, 10, 0

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

; ATTENTION! If code segment position is about to be changed changes must be made to idt.c

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
    resb STACK_SIZE    ; This reserves 8KBytes of memory
_sys_stack:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; EOF
