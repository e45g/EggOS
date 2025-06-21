#pragma once

#include <efi.h>

typedef struct {
    UINT32 magic;
    UINT32 memmap_count;
    UINT32 memmap_key;
    EFI_PHYSICAL_ADDRESS memory_map;

    UINT32 horizontal_resolution;
    UINT32 vertical_resolution;
    UINT32 pixels_per_scan_line;
    EFI_PHYSICAL_ADDRESS framebuffer_base;
    UINT64 framebuffer_size;
} BOOT_INFO;

EFI_STATUS get_memory_map(BOOT_INFO *boot_info);
EFI_STATUS key_wait();
