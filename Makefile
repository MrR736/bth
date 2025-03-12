UNAME_M := $(shell uname -m)

ifeq ($(UNAME_M), x86_64)
    CC = gcc
    ARCH := x86_64
    OUTPUT_DIR := .lib/x86_64
else ifneq ($(filter i386 i486 i586 i686, $(UNAME_M)),)
    CC := gcc -m32
    ARCH := x86
    OUTPUT_DIR := .lib/x86
else ifeq ($(UNAME_M), aarch64)
    CC = aarch64-linux-gnu-gcc
    ARCH := aarch64
    OUTPUT_DIR := .lib/aarch64
else ifeq ($(UNAME_M), mips64el)
    CC = mips64-linux-gnuabi64-gcc -mips64r2 -mabi=64
    ARCH := mips64el
    OUTPUT_DIR := .lib/mips64
else
    $(error Unsupported architecture: $(UNAME_M))
endif

SRC := bth.c

TARGET := bth

all: $(TARGET)

$(TARGET):
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(SRC) -o $(OUTPUT_DIR)/$(TARGET)
	ln -s $(OUTPUT_DIR)/$(TARGET) $(TARGET).$(ARCH)

arch:
	for arch in x86_64 i386 aarch64 mips64el; do $(MAKE) UNAME_M=$$arch; done

install:
	cp -r $(OUTPUT_DIR)/$(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(TARGET)

clean:
	rm -rf .lib
	for arch in x86_64 x86 aarch64 mips64el; do rm -f $(TARGET).$$arch clean; done
