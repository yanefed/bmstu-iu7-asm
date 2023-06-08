_MAKE_OUT_DIR_ := $(shell mkdir -p out)
_MAKE_DISASM_DIR_ := $(shell mkdir -p disassembly)

.PHONY: math_c math_asm math sin disassembly clean

math_c:
	@echo
	@echo "CPP: "
	g++ main_math.cpp -m80387 -mno-80387 -DSSE -O0 -o math_cpp.exe
	./math_cpp.exe

math_asm:
	@echo
	@echo "ASM: "
	g++ main_math.cpp -masm=intel -DASM -O0 -o math_asm.exe
	./math_asm.exe

math: math_c math_asm

sin:
	@echo
	g++ main_sin.cpp -masm=intel -o cmp_sin.exe
	./cmp_sin.exe

disassembly:
	g++ -c main_math.cpp -mno-80387 -DSSE -O0 -o out/math_c.o
	objdump -M intel-mnemonic -d out/math_c.o  > disassembly/math_c.txt

all: math sin disassembly

clean:
	rm -rf out/* disassembly/* *.exe
