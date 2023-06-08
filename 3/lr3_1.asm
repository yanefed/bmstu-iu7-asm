PUBLIC string


STK SEGMENT PARA STACK 'STACK'
    db 200 DUP (0)
STK ENDS

SEGDATA SEGMENT PARA COMMON 'DATA'
    max_size      db 100
    string_len    db 0
    string        db 100 DUP ("$")
SEGDATA ENDS

SEGCODE SEGMENT PARA PUBLIC 'CODE'
    assume CS:SEGCODE, DS:SEGDATA
main:
    mov ax, SEGDATA
    mov ds, ax

    mov ah, 08h
    int 21h

    mov ah, 9
    mov dl, 10
    int 21h
    mov dl, 13
    int 21h

    mov ax, 4c00h
    int 21h
SEGCODE ENDS
END main