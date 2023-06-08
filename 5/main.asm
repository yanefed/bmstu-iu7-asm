EXTRN input_unsigned_oct: far
EXTRN output_unsign_bin: far
EXTRN output_sign_hex: far

DSEG SEGMENT PARA PUBLIC 'DATA'
    	Menu db "Menu: ", 13, 10,
    			"1 - Number input in unsigned octal representation", 13, 10,
				"2 - Number output in unsigned binary representation", 13, 10,
				"3 - Number output in signed hexadecimal representation", 13, 10,
				"4 - Exit", 13, 10, '$'
	InputOptionMSG	db "Select the menu option: ", '$'
	MenuOption db 0
    OptionErrorMSG db "Incorrect option. Please, try again.", '$'
	Commands dd input_unsigned_oct, output_unsign_bin, output_sign_hex, exit 
	Endline db 13, 10, '$'
DSEG ENDS

SSEG SEGMENT PARA STACK 'STACK'
	db 512 dup(0)
SSEG ENDS

CSEG1 SEGMENT PARA PUBLIC 'CODE'
	assume CS:CSEG1, SS:SSEG, DS:DSEG

exit proc far
	mov ax, 4c00h 			 		
	int 21h
exit endp

main:
	mov ax, DSEG
	mov ds, ax					
	
	menu_loop:
		mov dx, OFFSET Endline
		mov ah, 09h
		int 21h
		
		mov dx, OFFSET Menu
		int 21h						

		mov dx, OFFSET InputOptionMSG
		int 21h						

		mov ah, 01h
		int 21h						
		mov bl, al					

		call NewlinePrint

		mov al, bl					
		sub al, '1'				

		cmp al, 3
		ja write_error_message				
		
		mov cl, 4					
		mul cl						
		mov si, ax	
		call Commands[si]			
		jmp skip_error_message

		write_error_message:
			mov dx, OFFSET Endline
			mov ah, 09h
			int 21h
			
			mov dx, OFFSET OptionErrorMSG
			int 21h				

			mov dx, OFFSET Endline
			int 21h
		
		skip_error_message:
			mov dx, OFFSET Endline
			mov ah, 09h
			int 21h

			jmp menu_loop
	

NewlinePrint PROC NEAR                   
    mov ah, 02h
    mov dl, 10
    int 21h
    ret  
NewlinePrint ENDP 

CSEG1 ENDS
END main
