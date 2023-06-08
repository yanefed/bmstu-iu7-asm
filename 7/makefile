$(shell mkdir -p out)

CC := gcc
CC_ASM := nasm
CFLAGS := -std=c99 -Wall -Werror -pedantic -Wextra -Wfloat-equal -fverbose-asm -Wfloat-conversion -Iinc -Wvla

LINKFLAGS := -lpthread

PREF_SRC := ./src/
PREF_INC := ./inc/
PREF_OUT := ./out/

all: run

app.exe: $(PREF_SRC)main.c $(PREF_SRC)my_strlen.c $(PREF_INC)my_strlen.h
	$(CC) $(CFLAGS) -c $(PREF_SRC)main.c  -m32  -o $(PREF_OUT)main.o
	$(CC) $(CFLAGS) -c $(PREF_SRC)my_strlen.c -m32 -o  $(PREF_OUT)my_strlen.o
	$(CC_ASM) -f elf32 $(PREF_SRC)my_strncpy.asm -o $(PREF_OUT)my_strncpy.o
	$(CC) $(PREF_OUT)main.o $(PREF_OUT)my_strlen.o $(PREF_OUT)my_strncpy.o -m32 -o app.exe $(LINKFLAGS)

.PHONY: run assembly clean 

run: app.exe
	./app.exe

assembly: $(PREF_SRC)my_strlen.c
	$(CC) $(CFLAGS) -S $(PREF_SRC)my_strlen.c -m32 -o $(PREF_OUT)mystrlen.s

clean :
	rm -Rf $(PREF_OUT) ./*.exe