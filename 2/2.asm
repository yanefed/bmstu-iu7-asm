StkSeg SEGMENT PARA STACK 'STACK'
 DB 200h DUP (?)
StkSeg ENDS

DataS SEGMENT WORD 'DATA'
 HelloMessage DB 13 ;курсор поместить в нач. строки
 DB 10 ;перевести курсор на нов. строку
 DB 'Hello, world !' ;текст сообщения
 DB '$' ;ограничитель для функции DOS
DataS ENDS

Code SEGMENT WORD 'CODE'
 ASSUME CS:Code, DS:DataS

DispMsg:
 mov CX, 3
 mov AX,DataS ;загрузка в AX адреса сегмента данных
 mov DS,AX ;установка DS
 mov DX,OFFSET HelloMessage ;DS:DX - адрес строки
 mov AH,9 ;АН=09h выдать на дисплей строку
 msg:
  int 21h ;вызов функции DOS
  loop msg

 mov AH,7 ;АН=07h ввести символ без эха
 INT 21h ;вызов функции DOS
 mov AH,4Ch ;АН=4Ch завершить процесс
 int 21h ;вызов функции DOS

Code ENDS
 END DispMsg