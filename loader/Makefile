OBJ_DIR = obj/
BIN_DIR = bin/
SRC_DIR = src/
ASMARGS = -f bin

VPATH = src:src/include:bin

loader.bin: loader.asm aliases.asm
	@echo ~~~~~ Building bootloader ~~~~~
	cd $(SRC_DIR); nasm $(ASMARGS) loader.asm -o $(BIN_DIR)loader.bin

.PHONY: clean
clean:
	rm -f $(BIN_DIR)*
