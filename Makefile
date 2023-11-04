CSOURCES = $(shell find ./src -type f -name '*.c')
ASMSOURCES = $(shell find ./src -type f -name '*.asm')
OBJECTS = $(CSOURCES:.c=.o)  $(ASMSOURCES:.asm=.o)

QEMU = qemu-system-i386
PREFIX = i686-elf-
CC = $(PREFIX)gcc
LD = $(PREFIX)gcc
ASM = nasm

CFLAGS = -std=gnu99 -ffreestanding -g
LDFLAGS = -ffreestanding -nostdlib -g
ASMFLAGS = -f elf

.PHONY: clean run

clean:
	rm -rf $(OBJECTS)

run: kernel-c
	$(QEMU) -kernel ./kernel

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) -T linker.ld $^ -o $@ -lgcc

kernel-c: kernel
	rm -rf $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $<
