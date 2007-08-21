# setting variables depending on OS
# SYS   - OS identifier (win/nix)
# DS    - directory separator
# PS    - path separator
# RM    - command for deleting files
ifeq ($(OS),Windows_NT)
	SYS     := win
	DS      := \\	# this is hack to set DS to `\`
	PS      := ;
	RM      := del /Q
	NUL     := NUL
else
	SYS     := nix
	DS      := /
	PS      := :
	RM      := rm -f
	NUL     := /dev/null
	SYS_CPPFLAGS  := -fno-stack-protector
endif

# this hides commands
# set E to empty string if you want to see commands used
# it may by done from command line: make E=
E := @

# directories
SRC_DIR := src
INC_DIR := $(SRC_DIR)$(DS)headers
OBJ_DIR := obj
BIN_DIR := bin

# directories with trailing directory separator
SRC := $(SRC_DIR)$(DS)
OBJ := $(OBJ_DIR)$(DS)
BIN := $(BIN_DIR)$(DS)

VPATH := $(SRC_DIR)$(PS)$(INC_DIR)$(PS)$(OBJ_DIR)$(PS)$(BIN_DIR)

COMPILER := g++

CPPFLAGS := -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions \
			-fleading-underscore -nostdinc -fno-builtin \
			-nostartfiles -nostdlib -fno-rtti -fno-exceptions \
			-c \
			$(SYS_CPPFLAGS)

# nasm flags
# on nix nasm requires include directory with trailing slash
# on win nasm ignores trailing slash but can not work with trailing backslash!
ASFLAGS := -f aout -I$(SRC_DIR)/
LDFLAGS := -T link.ld -s -x $(MAP)

define CPP_COMPILE
		@echo ~~~ Compiling $(@F) from $(filter %.cpp, $(^F))
		$(E)$(COMPILER) $(CPPFLAGS) -o $(OBJ)$(@F) $(addprefix $(SRC), $(filter %.cpp, $(^F)))
endef

kernel.bin: kentry.o cpp_runtime.o main.o io_port.o vga.o idt.o
	@echo ~~~ Linking $(^F)
	$(E)ld $(LDFLAGS) -Map $(BIN)kernel.map -o $(BIN)kernel.bin $(addprefix $(OBJ), $(^F))
	@echo ~~~ Done

kentry.o: kentry.asm isr.asm utils.asm
	@echo ~~~ Compiling entry
	$(E)nasm $(ASFLAGS) $(SRC)kentry.asm -o $(OBJ)kentry.o

# default rule for compiling CPP source files
# used only if no rule is defined explictly
%.o: %.cpp %.h global.h
	$(CPP_COMPILE)

.PHONY: clean vars
clean:
	@echo ~~~ Cleaning kernel
	$(E)$(RM) $(OBJ)*.o > $(NUL)
	$(E)$(RM) $(BIN)*.bin > $(NUL)
	@echo ~~~ Done
