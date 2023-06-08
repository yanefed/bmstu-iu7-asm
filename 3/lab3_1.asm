code_seg segment byte public 'code'
    assume cs:code_seg
    add al, 3
    mov ah, 02h
    mov dl, al
    int 21h
    mov ah, 4ch
    int 21h
code_seg ends
end