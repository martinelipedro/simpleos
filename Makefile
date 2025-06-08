CSOURCES = $(shell find ./src -type f -name '*.c')
ASMSOURCES = $(shell find ./src -type f -name '*.asm')
SSOURCES = $(shell find ./src -type f -name '*.s')

OBJECTS = $(CSOURCES:.c=.o)  $(ASMSOURCES:.asm=.o) $(SSOURCES:.s=.o)

QEMU = qemu-system-i386
PREFIX = $(HOME)/opt/cross/bin/i686-elf-
CC = $(PREFIX)gcc
LD = $(PREFIX)gcc
ASM = nasm

CFLAGS = -std=gnu99 -ffreestanding -g -Wno-stack-protector
LDFLAGS = -ffreestanding -nostdlib -g
ASMFLAGS = -f elf

.PHONY: clean run debug

clean:
	rm -rf $(OBJECTS)

run: kernel-c
	$(QEMU) -kernel ./kernel -D ./log.txt -monitor stdio -d int -M smm=off -no-reboot -m 4G

debug: kernel-c
	$(QEMU) -kernel ./kernel -D ./log.txt -monitor stdio -d int -M smm=off -no-reboot -m 2G -s -S

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) -T linker.ld $^ -o $@ -lgcc

kernel-c: kernel
	rm -rf $(OBJECTS)

image: kernel-c
	mkdir -p isodir/boot/grub
	cp kernel isodir/boot/kernel
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso isodir

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $<

%.o: %.s
	i686-elf-gcc -std=gnu99 -ffreestanding -g -c $< -o $@
