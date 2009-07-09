#EveOS linux core makefile

# this hides commands
# set E to empty string if you want to see commands used
# it may by done from command line: make E=
E := @

# setting directories
LOADER_DIR  := loader
KERNEL_DIR  := kernel
IMAGE_DIR   := image

KERNEL      := $(KERNEL_DIR)/bin/kernel.bin
LOADER      := $(LOADER_DIR)/bin/loader.bin
IMAGE       := $(IMAGE_DIR)/eveos.img

all:
	$(E)(cd $(LOADER_DIR) && $(MAKE))
	$(E)(cd $(KERNEL_DIR) && $(MAKE))
	@echo ~~~ Building EveOS image

	$(E)(cat $(LOADER) > $(IMAGE))
	$(E)(cat $(KERNEL) >> $(IMAGE))


.PHONY: clean build vars
clean:
	@echo ~~~ Cleaning image
	-$(E)(rm -f $(IMAGE) > /dev/null)
	-$(E)(cd $(KERNEL_DIR) && $(MAKE) clean)
	-$(E)(cd $(LOADER_DIR) && $(MAKE) clean)

build: clean all

vars:
	@echo E=$(E)
