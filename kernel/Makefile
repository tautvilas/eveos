
# nustatom intamuosius pagal sistema
ifeq ($(OS),Windows_NT)
	OS := win
	DS := \\# this is hack to set DS to `\`
	PS := ;
	RM := del /Q
else
	OS := nix
	DS := /
	PS := :
	RM := rm
endif

E := @

# nustatom direktorijas
SRC_DIR := src
INC_DIR := $(SRC_DIR)$(DS)headers
OBJ_DIR := obj
BIN_DIR := bin

SRC := $(SRC_DIR)$(DS)
OBJ := $(OBJ_DIR)$(DS)
BIN := $(BIN_DIR)$(DS)

VPATH := $(SRC_DIR)$(PS)$(INC_DIR)$(PS)$(OBJ_DIR)

CFLAGS := -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions \
		-ffreestanding -fleading-underscore -nostdinc -fno-builtin \
		-I$(INC_DIR) -c -x c
ASFLAGS := -f aout -I$(SRC_DIR)

define C_COMPILE
		@echo ~~~ Compiling $(@F) from $(filter %.c, $(^F))
		$Egcc $(CFLAGS) -o $(OBJ)$(@F) $(addprefix $(SRC), $(filter %.c, $(^F)))
endef

kernel.bin: kentry.o main.o gdt.o
	@echo ~~~ Linking $(^F)
	$Eld -T link.ld -o $(BIN)kernel.bin $(addprefix $(OBJ), $(^F))

kentry.o: kentry.asm aliases.asm
	@echo ~~~ Compiling entry
	$Enasm $(ASFLAGS) $(SRC)kentry.asm -o $(OBJ)kentry.o

main.o: main.c system.h
	$(C_COMPILE)

gdt.o: gdt.c system.h
	$(C_COMPILE)

.PHONY: clean test
clean:
	@echo ~~~ Cleaning
	-$E$(RM) $(OBJ)*.o
	-$E$(RM) $(BIN)*.bin

test:
	@echo OS=$(OS) DS=$(DS) PS=$(PS) $(PF) VPATH=$(VPATH) .SUFFIXES=$(.SUFFIXES) RM=$(RM)
