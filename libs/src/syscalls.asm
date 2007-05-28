bits 32

global _sys_write

SECTION .text

_sys_write:
    pusha
    mov     eax, 4  ; syscall id (sys_write)
    mov     ebx, 1  ; stdio
    mov     edx, [esp + 40]     ; string length
    mov     ecx, [esp + 36]     ; string offset
    int 69
    popa
    ret
