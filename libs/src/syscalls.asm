bits 32

global _sys_write
global _sys_read

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

_sys_read:
    pusha
    mov     eax, 3  ; syscall id (sys_read)
    mov     edx, [esp + 44]     ; numbytes to read
    mov     ecx, [esp + 40]     ; string offset
    mov     ebx, [esp + 36]     ; file descriptor
    int 69
    popa
    ret
