#EveOS core makefile

# setting variables depending on OS
# SYS   - OS identifier (win/nix)
# DS    - directory seperator
# PS    - path seperator
# RM    - command for deleting files
ifeq ($(OS),Windows_NT)
	SYS      := win
	DS      := \\# this is hack to set DS to `\`
	PS      := ;
	RM      := del /Q
else
	SYS      := nix
	DS      := /
	PS      := :
	RM      := rm
endif

E := @

# setting directories
KERNEL_DIR  := kernel
LOADER_DIR  := loader
IMAGE_DIR   := image

KERNEL      := $(KERNEL_DIR)$(DS)bin$(DS)kernel.bin
LOADER      := $(LOADER_DIR)$(DS)bin$(DS)loader.bin
IMAGE       := $(IMAGE_DIR)$(DS)floppy.img

VPATH   := $(IMAGE_DIR)$(PS)$(LOADER_DIR)$(DS)bin$(PS)$(KERNEL_DIR)$(DS)bin


$(IMAGE): $(LOADER) $(KERNEL)
	@echo ~~~ Preparing floppy image
ifeq ($(SYS),win)
	$Ecopy /Y $(LOADER) /B + $(KERNEL) /B $(IMAGE) /B > NUL
else
	$Edd if=$(LOADER) of=$(IMAGE) bs=512 count=1
	$Edd if=$(KERNEL) of=$(IMAGE) bs=512 seek=1
endif
	@echo ~~~ Done


$(LOADER):
	$E(cd $(LOADER_DIR) && $(MAKE))

$(KERNEL):
	$E(cd $(KERNEL_DIR) && $E$(MAKE))


.PHONY: clean
clean:
	@echo ~~~ Cleaning image
	-$E$(RM) $(IMAGE)
	@echo ~~~ Done
	-$E(cd $(KERNEL_DIR) && $(MAKE) clean)
	-$E(cd $(LOADER_DIR) && $(MAKE) clean)




# KERNEL_DIR = kernel/
# LOADER_DIR = loader/
# IMAGE_DIR = image/
# KERNEL_BIN = kernel/bin/kernel.bin
# LOADER_BIN = loader/bin/loader.bin
# IMAGE_BIN = image/floppy.img
#
# VPATH = image:kernel/bin:loader/bin:kernel/:loader
#
# all:
# 	(cd $(KERNEL_DIR) && ${MAKE})
# 	(cd $(LOADER_DIR) && ${MAKE})
# 	@echo ~~~~~ Preparing floppy image ~~~~~
# 	dd if=$(LOADER_BIN) of=$(IMAGE_BIN) bs=512 count=1
# 	dd if=$(KERNEL_BIN) of=$(IMAGE_BIN) bs=512 seek=1
#
# .PHONY: clean archive chekout
# clean:
# 	rm -f $(IMAGE_DIR)*.img
# 	(cd $(KERNEL_DIR) && make clean)
# 	(cd $(LOADER_DIR) && make clean)
#
# FILENAME=eveOS.`date +%Y-%m-%d`.tar
#
# archive:
# 	make clean
# 	tar -cvvf $(FILENAME) * --exclude archive
# 	gzip $(FILENAME)
# 	mv $(FILENAME).gz archive/
#
# chekout:
# 	(cd .. && ./checkout.sh)
