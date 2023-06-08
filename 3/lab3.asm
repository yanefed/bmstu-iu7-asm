stack_seg segment para stack 'stack'
    db 200 dup(0)
stack_seg ends

code_seg segment byte public 'code'
    assume ss:stack_seg, cs:code_seg
    main:
        mov ah, 08h
        int 21h
code_seg ends

end main
