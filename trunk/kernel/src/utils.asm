global _load_idt;
global _enable_int;
global _disable_int;

global _read_cr0;
global _read_cr2;
global _read_cr3;
global _write_cr0;
global _write_cr3;

global _read_eax;
global _read_ebx;
global _read_ecx;
global _read_edx;

global _load_tr;
global _read_tr;
global _read_tss_limit;

global _kernel_stop
global _kernel_idle

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; IDT                                                   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    retn

_enable_int:
    sti
    retn

_disable_int:
    cli
    retn

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Paging                                                ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_read_cr0:
    mov eax, cr0
    retn

_read_cr2:
    mov eax, cr2
    retn

_read_cr3:
    mov eax, cr3
    retn

_write_cr0:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]
    mov cr0, eax
    pop ebp
    retn

_write_cr3:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax
    pop ebp
    retn

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; TSS                                                   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_load_tr:
    mov ax, TSS_SEL
    ltr ax
    retn

_read_tr:
    xor eax, eax
    str ax
    retn

_read_tss_limit:
    mov ebx, TSS_SEL
    lsl eax, ebx
    jz read_tss_limit_success
    mov eax, 0
read_tss_limit_success:
    retn


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Misc                                                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_kernel_stop:
    jmp $

_kernel_idle:
    hlt
    jmp _kernel_idle
