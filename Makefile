LD = $(HOME)/opt/cross/bin/i686-elf-ld
GCC = $(HOME)/opt/cross/bin/i686-elf-gcc
ASM = nasm

SRC_DIR = src
KERNEL_DIR = $(SRC_DIR)/kernel
BOOT_DIR = $(SRC_DIR)/bootloader
BUILD_DIR = build
BIN_DIR = bin

KERNEL_C_SRCS := $(shell find $(KERNEL_DIR) -name '*.c')
KERNEL_ASM_SRCS := $(shell find $(KERNEL_DIR) -name '*.asm')

KERNEL_C_OBJS := $(addprefix $(BUILD_DIR)/,$(notdir $(KERNEL_C_SRCS:.c=_c.o)))
KERNEL_ASM_OBJS := $(addprefix $(BUILD_DIR)/,$(notdir $(KERNEL_ASM_SRCS:.asm=_asm.o)))

BOOT_BIN = $(BIN_DIR)/boot.bin
KERNEL_BIN = $(BIN_DIR)/kernel.bin
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
OS_IMAGE = $(BIN_DIR)/os.img

CFLAGS = -g3 -ffreestanding -O2 -Wall -Wextra -I$(KERNEL_DIR)/include/
LDFLAGS = -T$(SRC_DIR)/linker.ld -static -nostdlib --nmagic

VPATH = $(sort $(dir $(KERNEL_C_SRCS)) $(dir $(KERNEL_ASM_SRCS)))

# Debug information
$(info C Sources: $(KERNEL_C_SRCS))
$(info C Objects: $(KERNEL_C_OBJS))
$(info ASM Sources: $(KERNEL_ASM_SRCS))
$(info ASM Objects: $(KERNEL_ASM_OBJS))

.PHONY: all clean run boot kernel

all: directories boot kernel
	dd if=/dev/zero of=$(OS_IMAGE) bs=512 count=80
	dd if=$(BOOT_BIN) of=$(OS_IMAGE) bs=512 conv=notrunc
	dd if=$(KERNEL_BIN) of=$(OS_IMAGE) bs=512 seek=1 conv=notrunc

directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

run: all
	qemu-system-i386 -cpu pentium -m 1G -drive format=raw,file=$(OS_IMAGE) -s -no-reboot -monitor stdio

boot:
	$(ASM) -f bin -o $(BOOT_BIN) $(BOOT_DIR)/boot.asm

# Pattern rule for C files
$(BUILD_DIR)/%_c.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

# Pattern rule for assembly files
$(BUILD_DIR)/%_asm.o: %.asm
	$(ASM) -f elf32 -o $@ $<

kernel: $(KERNEL_C_OBJS) $(KERNEL_ASM_OBJS)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $^
	objcopy -O binary $(KERNEL_ELF) $(KERNEL_BIN)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)
