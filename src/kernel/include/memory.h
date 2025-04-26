#pragma once

#include "common.h"

#define MEMORY_MAP_ADDR 0x8000

#define PAGE_SIZE 4096
#define PAGE_MASK 0xFFFFF000  // remove flags
#define BITMAP_ADDRESS 0x9000

#define RECURSIVE_BASE (uintptr_t)(1023U * 0x400000U)
#define PD_VIRT ((uint32_t*)(RECURSIVE_BASE + (1023U * PAGE_SIZE)))

typedef enum {
    USABLE = 1,
    RESERVED,
    ACPI_RECLAIMABLE,
    ACPI_NVS,
    BAD_MEMORY
} region_type_t;

typedef enum {
    PRESENT = 1 << 0,
    READ_WRITE = 1 << 1,
    USER = 1 << 2,
} vmm_flags_t;

typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi;
}__attribute__((packed)) memory_map_entry_t;

extern void enable_paging();
void vmm_init(void);
void map_page(uint32_t vaddr, uint32_t paddr, uint32_t flags);
void unmap_page(uint32_t vaddr);
uint32_t get_mapping(uint32_t vaddr);
void *alloc_page();
void free_page(void *page);

void pmm_init(void);
void print_memory_map(void);

void pmm_mark_used(uintptr_t addr);
void pmm_mark_free(uintptr_t addr);
void *pmm_alloc_page();
void pmm_free_page(void* addr);
