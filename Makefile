# Toolchain
LD               = $(HOME)/opt/cross/bin/i686-elf-ld
GCC              = $(HOME)/opt/cross/bin/i686-elf-gcc
ASM              = nasm
OBJCOPY          = $(HOME)/opt/cross/bin/i686-elf-objcopy

# Directories
GNU_DIR          = $(HOME)/gnu-efi
SRC_DIR          = src
KERNEL_DIR       = $(SRC_DIR)/kernel
BOOT_DIR         = $(SRC_DIR)/bootloader
BUILD_DIR        = build
BIN_DIR          = bin

# Source files
KERNEL_C_SRCS    := $(shell find $(KERNEL_DIR)   -name '*.c')
KERNEL_ASM_SRCS  := $(shell find $(KERNEL_DIR)   -name '*.asm')

# Object files
KERNEL_C_OBJS    := $(addprefix $(BUILD_DIR)/,$(notdir $(KERNEL_C_SRCS:.c=_c.o)))
KERNEL_ASM_OBJS  := $(addprefix $(BUILD_DIR)/,$(notdir $(KERNEL_ASM_SRCS:.asm=_asm.o)))

KERNEL_ELF       = $(BUILD_DIR)/kernel.elf
KERNEL_BIN       = $(BIN_DIR)/kernel.bin

# Flags
CFLAGS           = -g3 -ffreestanding -O2 -Wall -Wextra -I$(KERNEL_DIR)/include/
LDFLAGS_KERNEL   = -static -nostdlib --nmagic -T$(SRC_DIR)/kernel.ld


BOOTLOADER_SOURCES = $(wildcard $(BOOT_DIR)/*.c)
BOOTLOADER_OBJECTS = $(BOOTLOADER_SOURCES:$(BOOT_DIR)/%.c=$(BUILD_DIR)/%.o)

# Search paths for %.c rules
VPATH = $(dir $(KERNEL_C_SRCS))

.PHONY: all clean run boot kernel

all: clean directories boot iso kernel
	
directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

run: all
	sudo qemu-system-i386 \
	-drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/ia32/OVMF_CODE.4m.fd \
	-drive if=pflash,format=raw,file=/usr/share/edk2/ia32/OVMF_VARS.4m.fd \
	-cdrom iso/bootable.iso \
	-m 1G \
	# -serial stdio

$(BUILD_DIR)/%.o: $(BOOT_DIR)/%.c
	$(GCC) -I$(GNU_DIR)/inc \
	    -fpic \
	    -ffreestanding \
	    -fno-stack-protector \
	    -fno-stack-check \
	    -fshort-wchar \
	    -maccumulate-outgoing-args \
	    -Wall \
	    -Wextra \
	    -c $< -o $@

boot: $(BOOTLOADER_OBJECTS)
	$(LD) -shared \
	   -Bsymbolic \
	   -L$(GNU_DIR)/ia32/lib \
	   -L$(GNU_DIR)/ia32/gnuefi \
	   -T$(GNU_DIR)/gnuefi/elf_ia32_efi.lds \
	   $(GNU_DIR)/ia32/gnuefi/crt0-efi-ia32.o \
	   $(BOOTLOADER_OBJECTS) \
	   -o $(BUILD_DIR)/boot.so \
	   -lgnuefi -lefi
	
	objcopy -j .text \
	        -j .sdata \
	        -j .data \
	        -j .rodata \
	        -j .dynamic \
	        -j .dynsym \
	        -j .rel \
	        -j .rela \
	        -j .rel.* \
	        -j .rela.* \
	        -j .reloc \
	        --target efi-app-ia32 \
	        --subsystem=10 \
	        $(BUILD_DIR)/boot.so $(BUILD_DIR)/boot.efi

iso:
	mkdir -p iso/EFI/BOOT
	cp $(BUILD_DIR)/boot.efi iso/EFI/BOOT/BOOTIA32.EFI
	
	dd if=/dev/zero of=iso/efi.img bs=1024 count=2880
	mkfs.vfat -F12 iso/efi.img
	mmd -i iso/efi.img ::/EFI
	mmd -i iso/efi.img ::/EFI/BOOT  
	mcopy -i iso/efi.img $(BUILD_DIR)/boot.efi ::/EFI/BOOT/BOOTIA32.EFI
	
	xorriso -as mkisofs \
	        -R \
	        -f \
	        -e efi.img \
	        -no-emul-boot \
	        -o iso/bootable.iso \
	        iso/

# Kernel: assemble asm, compile C, link, objcopy
$(BUILD_DIR)/%_asm.o: %.asm
	$(ASM) -f elf32 -o $@ $<

$(BUILD_DIR)/%_c.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

kernel: $(KERNEL_ASM_OBJS) $(KERNEL_C_OBJS)
	$(LD) $(LDFLAGS_KERNEL) -o $(KERNEL_ELF) $^
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_BIN)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) iso/

