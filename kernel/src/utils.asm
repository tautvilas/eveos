global _read_cr0;
global _read_cr3;
global _write_cr0;
global _write_cr3;

global _read_eax;
global _read_ebx;
global _read_ecx;
global _read_edx;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Paging                                                ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_read_cr0:
    mov eax, cr0
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
