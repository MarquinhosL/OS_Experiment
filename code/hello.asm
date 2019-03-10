[section .data]

strHello    db  "Hello WORLD!\n"
strLen  equ $-strHello

[section .text]
global _start

_start:
    mov edx, strLen
    mov ecx, strHello
    mov ebx, 0
    mov eax, 4 ;sys_write
    int 0x80
    mov ebx, 1
    mov eax, 1 ;sys_ext
    int 0x80
