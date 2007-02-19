SRC_DIR = src/
OBJ_DIR = obj/
BIN_DIR = bin/

VPATH = src:src/headers:bin/:obj/
CARGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -fleading-underscore -finline-functions \
		-ffreestanding -nostdinc -fno-builtin -I$(SRC_DIR)/headers -c
ASMARGS = -f aout -I$(SRC_DIR)

kernel.bin: kentry.o main.o gdt.o
	@echo ~~~~~ Linking kernel ~~~~~
	ld -T link.ld -o $(BIN_DIR)kernel.bin $(OBJ_DIR)kentry.o $(OBJ_DIR)main.o $(OBJ_DIR)gdt.o
kentry.o: kentry.asm aliases.asm
	nasm $(ASMARGS) $(SRC_DIR)kentry.asm -o $(OBJ_DIR)kentry.o
main.o: main.c system.h
	gcc $(CARGS) -o $(OBJ_DIR)main.o $(SRC_DIR)main.c
gdt.o: gdt.c system.h
	gcc $(CARGS) -o $(OBJ_DIR)gdt.o $(SRC_DIR)gdt.c

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)*
	rm -f $(BIN_DIR)*
