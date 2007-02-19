#EveOS core makefile

KERNEL_DIR = kernel/
LOADER_DIR = loader/
IMAGE_DIR = image/
KERNEL_BIN = kernel/bin/kernel.bin
LOADER_BIN = loader/bin/loader.bin
IMAGE_BIN = image/floppy.img

VPATH = image:kernel/bin:loader/bin:kernel/:loader

all:
	(cd $(KERNEL_DIR) && ${MAKE})
	(cd $(LOADER_DIR) && ${MAKE})
	@echo ~~~~~ Preparing floppy image ~~~~~
	dd if=$(LOADER_BIN) of=$(IMAGE_BIN) bs=512 count=1
	dd if=$(KERNEL_BIN) of=$(IMAGE_BIN) bs=512 seek=1

.PHONY: clean archive chekout
clean:
	rm -f $(IMAGE_DIR)*.img
	(cd $(KERNEL_DIR) && make clean)
	(cd $(LOADER_DIR) && make clean)

FILENAME=eveOS.`date +%Y-%m-%d`.tar

archive:
	make clean
	tar -cvvf $(FILENAME) * --exclude archive
	gzip $(FILENAME)
	mv $(FILENAME).gz archive/

chekout:
	(cd .. && ./checkout.sh)
