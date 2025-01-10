LINKER_SRC = src/linker.ld
KERNEL_SRC = src/kernel
BOOT_SRC = src/bootloader

BOOT_BIN = bin/boot.bin
KERNEL_BIN = bin/kernel.bin

KERNEL_OUT = build/kernel.o
KERNEL_ASM_OUT = build/kernel.asm.o
COMPLETE_KERNEL_OUT = build/complete_kernel.o

OS_BIN = bin/os.bin

FILES = $(KERNEL_ASM_OUT) $(KERNEL_OUT)
FLAGS = -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -O2 -lgcc

all: clean boot kernel

run: all
	qemu-system-i386 -drive format=raw,file=$(OS_BIN) -nographic

boot:
	nasm -f bin -o $(BOOT_BIN) $(BOOT_SRC)/boot.asm

kernel:
	nasm -f elf -g $(KERNEL_SRC)/kernel.asm -o $(KERNEL_ASM_OUT)
	$(HOME)/opt/cross/bin/i686-elf-gcc -Isrc $(FLAGS) -std=gnu99 -c $(KERNEL_SRC)/kernel.c -o $(KERNEL_OUT)
	$(HOME)/opt/cross/bin/i686-elf-ld -g -relocatable $(FILES) -o $(COMPLETE_KERNEL_OUT)
	$(HOME)/opt/cross/bin/i686-elf-gcc $(FLAGS) -T $(LINKER_SRC) -o $(KERNEL_BIN) -ffreestanding -O2 -nostdlib $(COMPLETE_KERNEL_OUT)

	dd if=$(BOOT_BIN) of=$(OS_BIN) bs=512 count=1 conv=notrunc
	dd if=$(KERNEL_BIN) of=$(OS_BIN) bs=512 seek=1 conv=notrunc
	dd if=/dev/zero bs=512 count=32 >> $(OS_BIN)

clean:
	rm -f $(BOOT_BIN)
	rm -f $(KERNEL_BIN)
	rm -f $(KERNEL_OUT)
	rm -f $(KERNEL_ASM_OUT)
	rm -f $(COMPLETE_KERNEL_OUT)
	rm -f $(OS_BIN)
