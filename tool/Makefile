ifeq ($(CHIP),)
CHIP_NAME = mt7626
else
CHIP_NAME = $(CHIP)
endif

EMI_DIR_PATH = $(shell cd ..;pwd)
BIN_IN_PATH = $(EMI_DIR_PATH)/bin
DIR_OUT_PATH = $(EMI_DIR_PATH)/include

export CHIP_NAME EMI_DIR_PATH BIN_IN_PATH DIR_OUT_PATH

all:
	gcc -g bin2h.c -o bin2h
	./bin2h
clean:
	rm -f *.o bin2h