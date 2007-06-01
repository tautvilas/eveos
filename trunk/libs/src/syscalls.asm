bits 32

global _sys_write
global _sys_read

SYS         equ 69
SYS_READ    equ 3

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
    ; pusha
    push    ebp
    mov     ebp, esp
    add     ebp, 4

    push    edx
    push    ecx
    push    ebx

    mov     eax, SYS_READ       ; syscall id (sys_read)
    mov     ebx, [ebp + 4]     ; file descriptor
    mov     ecx, [ebp + 8]     ; string offset
    mov     edx, [ebp + 12]     ; numbytes to read
    int     SYS

    pop     ebx
    pop     ecx
    pop     edx

    pop     ebp
    ; popa
    ret
