;;;;;;;;;;;;;;;;;;;;;;
; EveOS Kernel 0.0.1 ;
;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Init                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include "aliases.asm"

bits 16

global _start     ; entry symbol for linker
global _idt_load  ; function for loading IDT

extern _os_main           ; OS main C function
extern _gIdtp             ; Pointer to IDT
extern _exception_handler ; ISRs handler
extern _irq_handler       ; IRQs handler

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Code                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION .text

    ; this jmp is needed for kernel loader integrity test
    jmp _start
_start:

    ; print out OS welcome message
    mov eax, title - KERNEL_BASE
    mov si, ax
    call print_str - KERNEL_BASE

    cli             ; Disable external interrupts

    ; init gdt
    mov eax, gdtptr
    sub eax, KERNEL_BASE
    lgdt[eax]

    ; init p-mode
    mov eax, cr0    ; The lsb of cr0 is the protected mode bit
    or al, 1        ; Set protected mode bit
    mov cr0, eax    ; Mov modified word to the control register
    ; far jump is for setting code segment
    jmp dword CODE_SEL:go_pm - KERNEL_BASE

bits 32

go_pm:
    ; initialise all segments to data selector
    mov ax, DATA_SEL
    mov ds, ax
    mov fs, ax
    mov es, ax
    mov ss, ax
    mov gs, ax

    ; init paging
    mov esp, _sys_stack - KERNEL_BASE

    ; init kernel page table (over 2GB and first 4MB)
    mov eax, pd - KERNEL_BASE       ; eax  = &pd
    mov ebx, pt - KERNEL_BASE + PAGE_RW_PRESENT   ; ebx  = &pt | 3
    mov [eax], ebx                  ; pd[0] = &pt

    ; TODO:zv:fix this
    mov eax,  pd - KERNEL_BASE + PAGE_DIRECTORY_OFFSET ; eax = &PDE[960]
    mov [eax], ebx                  ; PD[960] = &PT

    ; fill in the page table

    mov edi, pt - KERNEL_BASE       ; edi = &PT
    mov eax, 3                      ; Address 0, bit p & r/w set
    mov ecx, 1024                   ; 1024 entries
init_pt:
    stosd                           ; Write one entry
    add eax, 1000h                  ; Next page address
    loop init_pt                    ; Loop

    ; set the page directory in cr3

    mov eax,  pd - KERNEL_BASE      ; eax = &PD
    mov cr3, eax                    ; cr3 = &PD

    ; set CR0's PG bit.

    mov eax, cr0
    or eax, 80000000h           ; Set PG bit
    mov cr0,eax                 ; Paging is on!
    jmp $+2                     ; Flush the instruction queue.

    jmp CODE_SEL:paging_enabled

    ;push paging_enabled         ;Keep full address (8000xxxxh)
    ;ret                         ;Jump at Paging Mode (below)

paging_enabled:

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

; Useful assembler functions, called from C code
%include "utils.asm"

; ISRs and IRQs function stubs
%include "isr.asm"

; Load the interrupt descriptor table
_idt_load:
    lidt [_gIdtp]
    ret

isr_common:
    cld
    pushad
    push ds
    push es
    push fs
    push gs
    mov ax, DATA_SEL
    mov ds, ax
    mov fs, ax
    mov es, ax
    mov gs, ax
    mov eax, esp
    push eax    ; pointer to regs struct
    mov eax, _exception_handler
    call eax    ; a special call, preserves 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8  ; cleans up pushed error code and ISR number
    iret        ; pops cs, eip, eflags, ss and esp

irq_common:
    pushad
    push ds
    push fs
    push es
    push gs
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax    ; pointer to regs struct
    mov eax, _irq_handler
    call eax    ; a special call, preserves 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8  ; cleans up pushed error code and ISR number
    iret        ; pops cs, eip, eflags, ss and esp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Data                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION .data

title:
    db 13, 10, "EveOS kernel v0.0.1 is starting, please fasten your seatbelts", 13, 10, 0

; main pointer to gdt
gdtptr :
    dw gdt_end-gdt-1   ; Length of the gdt - 1
    dd gdt - KERNEL_BASE      ; physical address of gdt
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
    dw 0x0000          ; BASE 0h bits 16-31 of segment descriptor (sd)
    db 0x00            ; BASE addr of seg 16-23 of 32bit addr,32-39 of sd
    db 10011010b       ; P,DPL(2),S,TYPE(3),A->Present bit 1,Descriptor
                       ; privilege level 0-3,Segment descriptor 1 ie code
                       ; or data seg descriptor,Type of seg,Accessed bit
    db 11001111b       ; Upper 4 bits G,D,0,AVL ->1 segment len is page
                       ; granular, 1 default operation size is 32bit seg
                       ; AVL : Available field for user or OS
                       ; Lower nibble bits 16-19 of segment limit
    db 0x00            ; BASE addr of seg 24-31 of 32bit addr,56-63 of sd
DATA_SEL equ $-gdt     ; ie 10h, beginning of next 8 bytes for data sd
data_gd                ; Data descriptor 4Gb flat seg at 0000:0000h
    dw 0x0ffff         ; Limit 4Gb
    dw 0x0000          ; BASE 0000:0000h
    db 0x00            ; Descriptor format same as above
    db 10010010b
    db 11001111b
    db 0x00
gdt_end

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BSS                                                   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION .bss
; page directory
pd:
    resb PAGE_SIZE

; page table
pt:
    resb PAGE_SIZE

; system stack
    resb STACK_SIZE    ; This reserves 8KBytes of memory
_sys_stack:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; EOF
