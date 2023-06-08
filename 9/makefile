$(shell mkdir -p out)

CC := gcc

CFLAGS := -std=c99 -fverbose-asm #-masm=intel #-fverbose-asm #-Wall -Werror -pedantic -Wextra  -Wfloat-equal -Iinc -Wvla #-Wfloat-conversion

PREF_SRC := ./src/
PREF_OUT := ./out/

# получаем список си файлов в подпапке ./src/
SRC := $(wildcard $(PREF_SRC)*.c)

# получаем заменой список объектных файлов для файлов из подпапки ./src/
SRC_OBJ := $(patsubst $(PREF_SRC)%.c, $(PREF_OUT)%.o, $(SRC))

all : app.exe
	./app.exe

app.exe : $(SRC_OBJ)
	$(CC) $(SRC_OBJ) -o app.exe 

# получаем объектные файлы с помощью си файлов из подпапки ./src/
$(PREF_OUT)%.o : $(PREF_SRC)%.c
	$(CC) $(CFLAGS) -c -MD $< -o $@

include $(wildcard $(PREF_OUT)*.d)

.PHONY: debug release clean 

debug : CFLAGS += -g3
debug : app.exe

release : CFLAGS += -DNDEBUG -g0
release : app.exe

assembly:
	gcc $(CFLAGS) -S $(PREF_SRC)main.c -O0 -Iinc -o $(PREF_OUT)main.s

clean : 
	rm -Rf $(PREF_OUT) ./*.exe 