OBJ_DIR = obj/
BIN_DIR = bin/
SRC_DIR = src/

ASMARGS = -f bin -I$(SRC_DIR)

VPATH = src:bin

loader.bin: loader.asm aliases.asm
	@echo ~~~~~ Building bootloader ~~~~~
	nasm $(ASMARGS) $(SRC_DIR)loader.asm -o $(BIN_DIR)loader.bin

.PHONY: clean
clean:
	rm -f $(BIN_DIR)*
