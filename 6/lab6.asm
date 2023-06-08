.MODEL tiny 

CSEG SEGMENT 
    assume CS:CSEG
    org 100h            

    main:
        jmp interrupt
        speed db 00011111b      

        is_interrupt db 1      
                                
        cur_time db 0           


    handler proc near
        pushf               
                            
        mov ah, 02h         ;чтение текущего времени из часов CMOS
                            ;СН - значение текущего часа.
                            ;CL - значение текущей минуты.
                            ;DH - значение текущей секунды.

        int 1ah             ;1ah - прерывание BIOS для работы с таймером.

        cmp dh, cur_time    
        je no_change_speed  

        mov cur_time, dh    
        dec speed           

        set_speed:          
            mov al, 0F3h    ;команда F3h указывает на режим автоповтора

            out 60h, al     ;порт 60h предназначен для работы с клавиатурой 
            
            mov al, speed
            out 60h, al     ;устанавливаем скорость автоповтора ввода.

        no_change_speed:
            popf            ;восстанавливаем значение регистра FLAGS из стека

            db 0EAh     ;возвращаем усправление ОС - длинная форма jmp far.

            old_handler dd  0   ;адрес старого обработчика прерываний  
   
    HANDLER endp

    INTERRUPT:       
        mov ax, 3508h   ;считали адрес обработчика прерывания таймера в BIOS          

        int 21h         ;адрес обработчика сохраняется в регистрах ES и BX
                        ;es - адрес сегмента
                        ;bx - смещение.

        cmp es:is_interrupt, 1      
        je RESTORE                  

        ;копируем в пер old_handler изначальный обработчик (смещение)
        mov word ptr old_handler, bx       

        ;копируем в пер old_handler изначальный обработчик (сегмент)  
        mov word ptr old_handler + 2, es    

        ;запрос на замену вектора прерывания в таблице векторов прерываний 
        ;на наш обработчик.
        mov ax, 2508h              
        mov dx, offset HANDLER
        int 21h                     

        mov dx, offset init_msg     
        mov ah, 09h
        int 21h                            

        mov ah, 27h                         
        mov dx, offset INTERRUPT            ; dx := адрес последнего байта программы (считая от начала PSP) + 1
        int 27h                             ; оставляем программу резидентной 


    RESTORE:
        mov dx, offset exit_msg             
        mov ah, 09h
        int 21h

        mov al, 0F3h    

        out 60h, al    

        ;устанавливаем период автоповтора 30.0, задержку включения режима автоповтора 250 мс (восстанавливаем дефолтные значения)
        mov al, 0
        out 60h, al                 
        
        ; устанавливаем период автоповтора 30.0, задержку включения режима автоповтора 250 мс (восстанавливаем дефолтные значения)
        mov dx, word ptr es:old_handler                       
        mov ds, word ptr es:old_handler + 2
        mov ax, 2508h
        int 21h                  

        mov ah, 49h                         
        int 21h

        mov ax, 4c00h
        int 21h 

        init_msg db 'INSTALLED INTERRUPT HANDLER.$'   
        exit_msg db 'UNINSTALLED INTERRUPT HANDLER.$'
CSEG ENDS
END MAIN
    