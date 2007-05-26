#EveOS core makefile

# setting variables depending on OS
# SYS   - OS identifier (win/nix)
# DS    - directory separator
# PS    - path separator
# RM    - command for deleting files
# NUL   -
ifeq ($(OS),Windows_NT)
	SYS     := win
	DS      := \\# this is hack to set DS to `\`
	PS      := ;
	RM      := del /Q
	NUL     := NUL
else
	SYS     := nix
	DS      := /
	PS      := :
	RM      := rm -f
	NUL     := /dev/null
endif

# this hides commands
# set E to empty string if you want to see commands used
# it may by done from command line: make E=
E := @

# setting directories
KERNEL_DIR  := kernel
LIBS_DIR    := libs
LOADER_DIR  := loader
IMAGE_DIR   := image
ESH_DIR     := esh

KERNEL      := $(KERNEL_DIR)$(DS)bin$(DS)kernel.bin
LOADER      := $(LOADER_DIR)$(DS)bin$(DS)loader.bin
IMAGE       := $(IMAGE_DIR)$(DS)eveos.img
ESH         := $(ESH_DIR)$(DS)bin$(DS)esh.bin

VPATH   := $(IMAGE_DIR)$(PS)$(LOADER_DIR)$(DS)bin$(PS)$(KERNEL_DIR)$(DS)bin


all:
	$(E)(cd $(LOADER_DIR) && $(MAKE))
	$(E)(cd $(KERNEL_DIR) && $(MAKE))
	$(E)(cd $(LIBS_DIR) && $(MAKE))
	$(E)(cd $(ESH_DIR) && $(MAKE))
	@echo ~~~ Building EveOS image
ifeq ($(SYS),win)
	$Ecopy /Y $(LOADER) /B + $(KERNEL) /B $(IMAGE) /B > $(NUL)
else
	$(E)cat $(LOADER) > $(IMAGE)
	$(E)cat $(KERNEL) >> $(IMAGE)
	$(E)cat $(ESH) >> $(IMAGE)
endif
	@echo ~~~ Done


.PHONY: clean vars
clean:
	@echo ~~~ Cleaning image
	-$E$(RM) $(IMAGE) > $(NUL)
	@echo ~~~ Done
	-$(E)(cd $(KERNEL_DIR) && $(MAKE) clean)
	-$(E)(cd $(LIBS_DIR) && $(MAKE) clean)
	-$(E)(cd $(LOADER_DIR) && $(MAKE) clean)
	-$(E)(cd $(ESH_DIR) && $(MAKE) clean)

vars:
	@echo OS=$(OS) DS=$(DS) PS=$(PS) RM=$(RM) E=$E
