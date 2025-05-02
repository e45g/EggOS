
# Toolchain
LD               = $(HOME)/opt/cross/bin/i686-elf-ld
GCC              = $(HOME)/opt/cross/bin/i686-elf-gcc
ASM              = nasm
OBJCOPY          = $(HOME)/opt/cross/bin/i686-elf-objcopy

# Directories
SRC_DIR          = src
KERNEL_DIR       = $(SRC_DIR)/kernel
BOOT_DIR1        = $(SRC_DIR)/bootloader/1
BOOT_DIR2        = $(SRC_DIR)/bootloader/2
BUILD_DIR        = build
BIN_DIR          = bin

# Source files
KERNEL_C_SRCS    := $(shell find $(KERNEL_DIR)   -name '*.c')
KERNEL_ASM_SRCS  := $(shell find $(KERNEL_DIR)   -name '*.asm')

# Object files
KERNEL_C_OBJS    := $(addprefix $(BUILD_DIR)/,$(notdir $(KERNEL_C_SRCS:.c=_c.o)))
KERNEL_ASM_OBJS  := $(addprefix $(BUILD_DIR)/,$(notdir $(KERNEL_ASM_SRCS:.asm=_asm.o)))

# Binaries
BOOT1_BIN        = $(BIN_DIR)/stage1.bin
BOOT2_BIN        = $(BIN_DIR)/stage2.bin
KERNEL_ELF       = $(BUILD_DIR)/kernel.elf
KERNEL_BIN       = $(BIN_DIR)/kernel.bin
OS_IMAGE         = $(BIN_DIR)/os.img

# Flags
CFLAGS           = -g3 -ffreestanding -O2 -Wall -Wextra -I$(KERNEL_DIR)/include/
LDFLAGS_KERNEL   = -static -nostdlib --nmagic -T$(SRC_DIR)/kernel.ld

# Search paths for %.c rules
VPATH = $(dir $(KERNEL_C_SRCS))

.PHONY: all clean run boot kernel

all: directories boot kernel
	dd if=/dev/zero of=$(OS_IMAGE) bs=512 count=100
	dd if=$(BOOT1_BIN) of=$(OS_IMAGE) bs=512 conv=notrunc
	dd if=$(BOOT2_BIN) of=$(OS_IMAGE) bs=512 seek=1 conv=notrunc
	dd if=$(KERNEL_BIN) of=$(OS_IMAGE) bs=512 seek=4 conv=notrunc

directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

run: all
	qemu-system-i386 -cpu pentium -m 1G -drive format=raw,file=$(OS_IMAGE) -s -no-reboot -monitor stdio

boot: $(BOOT1_BIN) $(BOOT2_BIN)

# Stage 1: raw boot sector
$(BOOT1_BIN): $(BOOT_DIR1)/stage1.asm
	$(ASM) -f bin -o $@ $<

# Stage 2: pure assembly flat binary
$(BOOT2_BIN): $(BOOT_DIR2)/stage2.asm
	$(ASM) -f bin -o $@ $<

# Kernel: assemble asm, compile C, link, objcopy
$(BUILD_DIR)/%_asm.o: %.asm
	$(ASM) -f elf32 -o $@ $<

$(BUILD_DIR)/%_c.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

kernel: $(KERNEL_ASM_OBJS) $(KERNEL_C_OBJS)
	$(LD) $(LDFLAGS_KERNEL) -o $(KERNEL_ELF) $^
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_BIN)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)


