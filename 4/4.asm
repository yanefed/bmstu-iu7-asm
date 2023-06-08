SSEG SEGMENT para STACK 'STACK'
	db 100 dup(0)
SSEG ENDS

DSEGMATRIX SEGMENT para public 'DATA'
	N db 0
    N_MAX db 9
	MATRIX db 9 * 9 dup(0)
DSEGMATRIX ENDS

DSEGMSG SEGMENT para public 'DATA'
	MATRIXMSG db 'Enter matrix:$'
    NMSG db 'Enter N:$'
    RESULTMSG db 'RESULT:$'
DSEGMSG ENDS

CSEG SEGMENT para public 'DATA'
    assume CS:CSEG
    assume SS:SSEG
    assume DS:DSEGMSG
    assume ES:DSEGMATRIX

msg_print proc                                  
    mov ax, dx               
    mov ah, 9                
    int 21h                  
    ret                      
msg_print endp

newline_print proc           
    mov ah, 02h              
    mov dl, 10              
    int 21h                  
    mov dl, 13               
    int 21h                  
    ret                     
newline_print endp

print_symbol proc                   
    push dx                  
    mov ah, 02h             
    mov dl, ' '              
    int 21h                  
    pop dx                   
	mov dl, dh 
	int 21h
    ret
print_symbol endp

read_symbol proc            ; процедура считывания числа
    mov ah, 02h
    mov dl, ' '
    int 21h

    mov ah, 01h
    int 21h
    mov dh, al 
    ret
read_symbol endp

check_hex_main proc
    cmp dh, '0'
    jg check_numbers
    ret
check_hex_main endp

check_numbers proc
    cmp dh, 'F'
    jle change_hex
    call check_uppercase
    ret
check_numbers endp

check_uppercase proc
    cmp dh, 'a'
    jge check_lowercase
    ret
check_uppercase endp

check_lowercase proc
    cmp dh, 'f'
    jle change_hex
    ret
check_lowercase endp

change_hex proc
    mov dh, '@'
    ret
change_hex endp

read_matrix proc           
    xor cx, cx            
    xor bx, bx
    xor si, si

    mov cl, N
    
    cmp cx, 9
    jg out_proc
    
    read_row_loop:         
        push cx            
        mov cl, N
        xor si, si
        
        read_element_loop:       
            call read_symbol
            call check_hex_main
            mov matrix[bx][si], dh      ; записываем число в матрицу из dh, так как прочитали туда
            inc si                     
            loop read_element_loop     
        call newline_print              
        add bl, N_MAX                    
           
        pop cx                          
        loop read_row_loop              
       
    ret
read_matrix endp

print_matrix proc          
    xor cx, cx            
    xor bx, bx
    xor si, si

    mov cl, N    
    
    cmp cx, 9
    jg out_proc
        
    print_row_loop:
        push cx
        mov cl, N
        mov si, 0
        
        print_element_loop:
            mov dh, matrix[bx][si]        ; в dh кладем само значение чтобы предать в print_matrix
            call print_symbol
            inc si
            loop print_element_loop
        call newline_print
        add bl, N_MAX        
        pop cx
        loop print_row_loop
        
    ret
print_matrix endp

out_proc:   
    ret


main:        
    mov ax, DSEGMSG
    mov ds, ax 
    
    mov ax, DSEGMATRIX
    mov es, ax
        
    mov dx, offset NMSG
    call msg_print
    
    call read_symbol
    sub dh, '0'
    mov N, dh    
    call newline_print
    
    mov dx, offset MATRIXMSG
    call msg_print
    call newline_print
    
    call read_matrix
    
    mov dx, offset RESULTMSG
    call msg_print
    call newline_print
    
    call print_matrix    
    
    mov ax, 4c00h
    int 21h
CSEG ENDS
END main