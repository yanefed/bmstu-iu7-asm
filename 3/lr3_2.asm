PUBLIC newline
PUBLIC output

EXTRN string: far

STK SEGMENT PARA STACK 'STACK'
    db 200 DUP (0)
STK ENDS

SEGDATA SEGMENT PARA COMMON 'DATA'
SEGDATA ENDS

SEGCODE SEGMENT PARA PUBLIC 'CODE'
    assume CS:SEGCODE, DS:SEGDATA, SS:STK
output proc near

    mov dl, byte ptr[string]
    mov ah, 2

    symb_label:
        int 21h
        mov dl, 32
        int 21h
        add bx, 1
        mov dl, byte ptr[string + bx]
        loop symb_label

    ret
output endp

newline proc near
    mov ah, 9
    mov dl, 10
    int 21h
    mov dl, 13
    int 21h

    ret
newline endp

SEGCODE ENDS
END