#pragma once

#include <common.h>

typedef struct {
    uint32_t magic;
    uint32_t memmap_count;
    uint32_t memmap_ptr;
    uint8_t boot_drive;
    uint8_t padding[3];
} __attribute__((packed)) bootinfo_t;

void get_bootinfo(bootinfo_t *info);
