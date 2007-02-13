SRC_DIR = src/
OBJ_DIR = obj/
BIN_DIR = bin/
VPATH = src:src/headers:src/include:bin/:obj/
CARGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc \
		-fno-builtin -I./headers -c
ASMARGS = -f aout

kernel.bin: kentry.o main.o
	@echo ~~~~~ Linking kernel ~~~~~
	ld -T link.ld -o $(BIN_DIR)kernel.bin $(OBJ_DIR)kentry.o $(OBJ_DIR)main.o
kentry.o: kentry.asm aliases.asm
	cd $(SRC_DIR); nasm $(ASMARGS) kentry.asm -o $(OBJ_DIR)kentry.o
main.o: main.c system.h
	cd $(SRC_DIR);gcc $(CARGS) -o $(OBJ_DIR)main.o main.c

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)*
	rm -f $(BIN_DIR)*
