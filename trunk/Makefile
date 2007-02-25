#EveOS core makefile

# setting variables depending on OS
# SYS   - OS identifier (win/nix)
# DS    - directory separator
# PS    - path separator
# RM    - command for deleting files
ifeq ($(OS),Windows_NT)
	SYS     := win
	DS      := \\# this is hack to set DS to `\`
	PS      := ;
	RM      := del /Q
else
	SYS     := nix
	DS      := /
	PS      := :
	RM      := rm -f
endif

E := @

# setting directories
KERNEL_DIR  := kernel
LOADER_DIR  := loader
IMAGE_DIR   := image

KERNEL      := $(KERNEL_DIR)$(DS)bin$(DS)kernel.bin
LOADER      := $(LOADER_DIR)$(DS)bin$(DS)loader.bin
IMAGE       := $(IMAGE_DIR)$(DS)eveos.img

VPATH   := $(IMAGE_DIR)$(PS)$(LOADER_DIR)$(DS)bin$(PS)$(KERNEL_DIR)$(DS)bin

all:
	$E(cd $(KERNEL_DIR) && $(MAKE))
	$E(cd $(LOADER_DIR) && $(MAKE))
	@echo ~~~ Preparing floppy image
ifeq ($(SYS),win)
	$Ecopy /Y $(LOADER) /B + $(KERNEL) /B $(IMAGE) /B > NUL
else
	$Edd if=$(LOADER) of=$(IMAGE) bs=512 count=1
	$Edd if=$(KERNEL) of=$(IMAGE) bs=512 seek=1
endif
	@echo ~~~ Done

.PHONY: clean
clean:
	@echo ~~~ Cleaning image
	-$E$(RM) $(IMAGE)
	@echo ~~~ Done
	-$E(cd $(KERNEL_DIR) && $(MAKE) clean)
	-$E(cd $(LOADER_DIR) && $(MAKE) clean)
