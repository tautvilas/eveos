;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Externalization                                       ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; ISR references

global _exc0    ; division by zero exception
global _exc1    ; debug exception
global _exc2    ; non maskable interrupt exception
global _exc3    ; breakpoint exception
global _exc4    ; into detected overflow exception
global _exc5    ; out of bounds exception
global _exc6    ; invalid opcode exception
global _exc7    ; no coprocessor exception
global _exc8    ; double fault exception (pushes error code)
global _exc9    ; coprocessor segment overrun exception
global _exc10   ; bad TSS exception (pushes error code)
global _exc11   ; segment not present exception (pushes error code)
global _exc12   ; stack fault exception (pushes error code)
global _exc13   ; general protection fault exception (pushes error code)
global _exc14   ; page fault exception (pushes error code)
global _exc15   ; unknown interrupt exception
global _exc16   ; coprocessor fault exception
global _exc17   ; aligment check exception (486+)
global _exc18   ; machine check exception (pentium/586+)

; reserved exceptions

global _exc19
global _exc20
global _exc21
global _exc22
global _exc22
global _exc23
global _exc24
global _exc25
global _exc26
global _exc27
global _exc28
global _exc29
global _exc30
global _exc31

; IRQs begin here

global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

; Kernel service interrupt

global _sys69

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Implementation                                        ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; ISRS

_exc0:
    cli
    push byte 0    ; A normal ISR stub that pops a dummy error code to keep a
                   ; uniform stack frame
    push byte 0
    jmp isr_common

_exc1:
    cli
    push byte 0
    push byte 1
    jmp isr_common

_exc2:
    cli
    push byte 0
    push byte 2
    jmp isr_common

_exc3:
    cli
    push byte 0
    push byte 3
    jmp isr_common

_exc4:
    cli
    push byte 0
    push byte 4
    jmp isr_common

_exc5:
    cli
    push byte 0
    push byte 5
    jmp isr_common

_exc6:
    cli
    push byte 0
    push byte 6
    jmp isr_common

_exc7:
    cli
    push byte 0
    push byte 7
    jmp isr_common

_exc8:
    cli
    push byte 8
    jmp isr_common

_exc9:
    cli
    push byte 0
    push byte 9
    jmp isr_common

_exc10:
    cli
    push byte 10
    jmp isr_common

_exc11:
    cli
    push byte 11
    jmp isr_common

_exc12:
    cli
    push byte 12
    jmp isr_common

_exc13:
    cli
    push byte 13
    jmp isr_common

_exc14:
    cli
    push byte 14
    jmp isr_common

_exc15:
    cli
    push byte 0
    push byte 15
    jmp isr_common

_exc16:
    cli
    push byte 0
    push byte 16
    jmp isr_common

_exc17:
    cli
    push byte 0
    push byte 17
    jmp isr_common

_exc18:
    cli
    push byte 0
    push byte 18
    jmp isr_common

_exc19:
    cli
    push byte 0
    push byte 19
    jmp isr_common

_exc20:
    cli
    push byte 0
    push byte 20
    jmp isr_common

_exc21:
    cli
    push byte 0
    push byte 21
    jmp isr_common

_exc22:
    cli
    push byte 0
    push byte 22
    jmp isr_common

_exc23:
    cli
    push byte 0
    push byte 23
    jmp isr_common

_exc24:
    cli
    push byte 0
    push byte 24
    jmp isr_common

_exc25:
    cli
    push byte 0
    push byte 25
    jmp isr_common

_exc26:
    cli
    push byte 0
    push byte 26
    jmp isr_common

_exc27:
    cli
    push byte 0
    push byte 27
    jmp isr_common

_exc28:
    cli
    push byte 0
    push byte 28
    jmp isr_common

_exc29:
    cli
    push byte 0
    push byte 29
    jmp isr_common

_exc30:
    cli
    push byte 0
    push byte 30
    jmp isr_common

_exc31:
    cli
    push byte 0
    push byte 31
    jmp isr_common

; IRQs begin here

_irq0:
    cli
    push byte 0 ; we need dummy byte here
    push byte 32
    jmp isr_common

_irq1:
    cli
    push byte 0 ; we need dummy byte here
    push byte 33
    jmp isr_common

_irq2:
    cli
    push byte 0 ; we need dummy byte here
    push byte 34
    jmp isr_common

_irq3:
    cli
    push byte 0 ; we need dummy byte here
    push byte 35
    jmp isr_common

_irq4:
    cli
    push byte 0 ; we need dummy byte here
    push byte 36
    jmp isr_common

_irq5:
    cli
    push byte 0 ; we need dummy byte here
    push byte 37
    jmp isr_common

_irq6:
    cli
    push byte 0 ; we need dummy byte here
    push byte 38
    jmp isr_common

_irq7:
    cli
    push byte 0 ; we need dummy byte here
    push byte 39
    jmp isr_common

_irq8:
    cli
    push byte 0 ; we need dummy byte here
    push byte 40
    jmp isr_common

_irq9:
    cli
    push byte 0 ; we need dummy byte here
    push byte 41
    jmp isr_common

_irq10:
    cli
    push byte 0 ; we need dummy byte here
    push byte 42
    jmp isr_common

_irq11:
    cli
    push byte 0 ; we need dummy byte here
    push byte 43
    jmp isr_common

_irq12:
    cli
    push byte 0 ; we need dummy byte here
    push byte 44
    jmp isr_common

_irq13:
    cli
    push byte 0 ; we need dummy byte here
    push byte 45
    jmp isr_common

_irq14:
    cli
    push byte 0 ; we need dummy byte here
    push byte 46
    jmp isr_common

_irq15:
    cli
    push byte 0 ; we need dummy byte here
    push byte 47
    jmp isr_common

; kernel service interrupt stub

_sys69:
    cli
    push byte 0
    push byte 69
    jmp isr_common

