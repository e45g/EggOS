#pragma once

#include "common.h"

#define MEMORY_MAP_ADDR 0x9000

#define PAGE_SIZE 4096
#define PAGE_MASK 0xFFFFF000  // remove flags
#define BITMAP_ADDRESS 0x10000

#define RECURSIVE_BASE (uintptr_t)(1023U * 0x400000U)
#define PD_VIRT ((uint32_t*)(RECURSIVE_BASE + (1023U * PAGE_SIZE)))

// heap - splits node if its is bigger than MIN_SPLIT + req_size
#define MIN_SPLIT 0x10

typedef enum {
    USABLE = 1,
    RESERVED,
    ACPI_RECLAIMABLE,
    ACPI_NVS,
    BAD_MEMORY
} region_type_t;

typedef enum {
    VMM_PRESENT = 1 << 0,
    VMM_RW = 1 << 1,
    VMM_USER = 1 << 2,
    VMM_PWT = 1 << 3,
    VMM_DISABLE_CACHE = 1 << 4
} vmm_flags_t;

typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi;
} __attribute__((packed)) memory_map_entry_t;

typedef struct heap_node {
    size_t size;
    bool is_free;

    struct heap_node *next;
    struct heap_node *prev;
} heap_node_t;

void pmm_init(void);
void pmm_mark_used(uintptr_t addr);
void pmm_mark_free(uintptr_t addr);
void *pmm_alloc_page();
void pmm_free_page(void* addr);
void print_memory_map(void);

void vmm_init(void);
int map_page(uint32_t vaddr, uint32_t paddr, uint32_t flags);
void unmap_page(uint32_t vaddr);
uint32_t get_mapping(uint32_t vaddr);
void *alloc_page();
void free_page(void *page);
extern void enable_paging();

void heap_init(void);
void heap_print();
void *malloc(size_t size);
void free(void *ptr);
