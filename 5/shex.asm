EXTRN Number: word

PUBLIC output_sign_hex

DSEG SEGMENT PARA PUBLIC 'DATA'
	OutputHexMSG        db "Signed hexadecimal number representation: ", '$'
	Endline             db 13, 10, '$'
DSEG ENDS

CSEG4 SEGMENT PARA PUBLIC 'CODE'
	assume CS:CSEG4, DS:DSEG

output_sign_hex proc far
	mov dx, OFFSET OutputHexMSG     
	mov ah, 09h
	int 21h

    	mov dx, OFFSET Endline
	int 21h

    	mov bx, Number           

    	cmp bx, 7FFFh                     
    	jna skip_sign

    	mov dl, '-'
    	mov ah, 02h
    	int 21h                                 
    	neg bx                                  

    	skip_sign:
    	mov cl, 12
    	ror bx, cl		; выполняем циклический сдвиг на 12 разрядов вправо, чтобы старшие 4 разряда числа оказались в конце числа

    	mov cx, 4                               
    	mov ah, 02h            

    	output_digits_loop:
			mov dl, bl                      ; помещаем последние две шестнадцатеричные цифры в регистр dl
			and dl, 0Fh                     ; зануляем (стираем) старшую цифру так мы выделили последнюю цифру числа

			cmp dl, 9                       
			ja make_letter

			add dl, '0'                    	
			jmp output_digit

			make_letter:
			sub dl, 10d
			add dl, 'A'                    	

			output_digit:
			int 21h         

			mov dx, cx                      
			mov cl, 4
			rol bx, cl		; сдвигаем число на 4 двоичных разряда влево (следующая шестнадцатеричная цифра теперь стоит в конце числа)
			mov cx, dx                     
	    	loop output_digits_loop
    
	mov dx, OFFSET Endline
	mov ah, 09h
	int 21h

    	ret
output_sign_hex endp

CSEG4 ENDS
END