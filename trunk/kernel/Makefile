# EveOS kernel linux makefile

# this hides commands
# set E to empty string if you want to see commands used
# it may by done from command line: make E=
E := @

# directories
SRC_DIR := src
INC_DIR := $(SRC_DIR)/headers
OBJ_DIR := obj
BIN_DIR := bin

# directories with trailing directory separator
SRC := $(SRC_DIR)/
OBJ := $(OBJ_DIR)/
BIN := $(BIN_DIR)/

VPATH := $(SRC_DIR):$(INC_DIR):$(OBJ_DIR):$(BIN_DIR)

COMPILER := g++

CPPFLAGS := -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions \
			-fleading-underscore -nostdinc -fno-builtin \
			-nostartfiles -nostdlib -fno-rtti -fno-exceptions \
			-I$(SRC_DIR) -c

# nasm flags
# on nix nasm requires include directory with trailing slash
# on win nasm ignores trailing slash but can not work with trailing backslash!
ASFLAGS := -f aout -I$(SRC_DIR)/
LDFLAGS := -T link.ld -s -x $(MAP)

MAP_DEMANGLE := awk 'BEGIN { hex = "^0x[0-9a-f]+$" } { \
        if ($2 ~ hex && $3 ~ hex) { addr = $2; size = $3 } \
        else if ($1 ~ hex && $2 ~ hex) { addr = $1; size = $2 } \
        else { size = "" } \
        if (size != "") { sub(addr " *" size, sprintf("%s %10d", addr, size)) } \
        print \
        }' | c++filt -_

define CPP_COMPILE
		@echo ~~~ Compiling $(@F) from $(filter %.cpp, $(^F))
		$(E)$(COMPILER) $(CPPFLAGS) -o $(OBJ)$(@F) $(addprefix $(SRC), $(filter %.cpp, $(^F)))
endef

kernel.bin: kentry.o cpp_runtime.o main.o \
        io_port.o \
        vga_caret_reg.o vga_caret.o vga.o \
        stream.o out_stream.o out.o \
#        idt.o \
#        mem.o
	@echo ~~~ Linking $(^F)
	$(E)ld $(LDFLAGS) -Map $(BIN)kernel.map -o $(BIN)kernel.bin $(addprefix $(OBJ), $(^F))
	$(E)cat $(BIN)kernel.map | c++filt -_ > $(BIN)kernel.readable.map
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
	$(E)rm -f $(OBJ)*.o > /dev/null
	$(E)rm -f $(BIN)*.bin > /dev/null
	$(E)rm -f $(BIN)*.map > /dev/null
	@echo ~~~ Done
