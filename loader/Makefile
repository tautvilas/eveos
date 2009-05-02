# this hides commands
# set E to empty string if you want to see commands used
# it may by done from command line: make E=
E := @

# directories
SRC_DIR := src
BIN_DIR := bin

# directories with trailing directory separator
SRC := $(SRC_DIR)/
OBJ := $(OBJ_DIR)/
BIN := $(BIN_DIR)/

VPATH := $(SRC_DIR):$(BIN_DIR)

# nasm flags
ASFLAGS := -f bin -I$(SRC_DIR)/

loader.bin: loader.asm
	@echo ~~~ Buildng loader
	$Enasm $(ASFLAGS) $(SRC)loader.asm -o $(BIN)loader.bin


.PHONY: clean
clean:
	@echo ~~~ Cleaning loader
	-$Erm -f $(BIN)*.bin > /dev/null

