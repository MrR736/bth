UNAME_M := $(shell uname -m)

# Compiler selection based on architecture
ifeq ($(UNAME_M), x86_64)
    CC = gcc
else ifneq ($(filter i386 i486 i586 i686, $(UNAME_M)),)
    CC := gcc -m32
else ifeq ($(UNAME_M), aarch64)
    CC = aarch64-linux-gnu-gcc
else ifeq ($(UNAME_M), mips64el)
    CC = mips64-linux-gnuabi64-gcc -mips64r2 -mabi=64
else
    $(error Unsupported architecture: $(UNAME_M))
endif

SRC := bth.c

TARGET := bth

# Default build target
all: $(TARGET)

$(TARGET):
	$(CC) $(SRC) -o $(TARGET)

arch:
	for arch in x86_64 i386 aarch64 mips64el; do $(MAKE) UNAME_M=$$arch; done

clean:
	rm -f $(TARGET)
