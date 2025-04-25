#pragma once

#include "common.h"

#define MEMORY_MAP_ADDR 0x8000

typedef enum {
    RAM = 1,
    RESERVED,
    ACPI_RECLAIMABLE,
    ACPI_NVS,
    BAD_MEMORY
} region_type_t;

typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;  // entry Type
    uint32_t acpi;  // extended
}__attribute__((packed)) memory_map_entry_t;

void print_memory_map();
