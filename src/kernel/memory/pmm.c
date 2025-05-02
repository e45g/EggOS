#include "kernel.h"
#include <keyboard.h>
#include <memory.h>
#include <common.h>

static uint32_t total_pages;
uint8_t* pmm_bitmap;

void pmm_init(void) {
    uint32_t total_mem = 0;

    const uint16_t size = boot_info.memmap_count;
    const memory_map_entry_t *map = (memory_map_entry_t*) ((uintptr_t)boot_info.memmap_ptr);

    for(int i = 0; i < size; i++) {
        memory_map_entry_t entry = map[i];
        if(entry.type == 1) {
            total_mem += entry.length;
        }
    }

    total_pages = total_mem / PAGE_SIZE;

    pmm_bitmap = (uint8_t*)BITMAP_ADDRESS;
    uint32_t bitmap_size = total_pages / 8;

    memset(pmm_bitmap, 0xFF, bitmap_size);

    for(int i = 0; i < size; i++) {
        memory_map_entry_t entry = map[i];
        if(entry.type == 1) {
            for(uint32_t addr = entry.base; addr < (entry.base+entry.length); addr += PAGE_SIZE) {
                if(addr >= kernel_end + 0x1000) {
                    pmm_mark_free(addr);
                }
            }
        }
    }
}

void pmm_mark_used(uintptr_t addr) {
    uint32_t page = addr / PAGE_SIZE;
    pmm_bitmap[page / 8] |= (1 << (page % 8));
}

void pmm_mark_free(uintptr_t addr) {
    uint32_t page = addr / PAGE_SIZE;
    pmm_bitmap[page / 8] &= ~(1 << (page % 8));
}

void *pmm_alloc_page() {
    for(uint32_t i = 0; i < total_pages; i++) {
        if(!(pmm_bitmap[i/8] & (1<<(i%8)))) {
            printf("PAGE_ALLOC: 0x%x\n", (uintptr_t)i * PAGE_SIZE);
            pmm_mark_used(i * PAGE_SIZE);
            return (void*)(i * PAGE_SIZE);
        }
    }
    PANIC("PMM - out of memory");
    return NULL;
}

void pmm_free_page(void* addr) {
    pmm_mark_free((uintptr_t)addr);
}


void print_memory_map() {
    const uint16_t size = boot_info.memmap_count;
    const memory_map_entry_t *map = (memory_map_entry_t *)((uintptr_t)boot_info.memmap_ptr);

    printf("Memory Map Size: %d\n", size);

    for (int i = 0; i < size; i++) {
        memory_map_entry_t entry = map[i];
        int type = entry.type;
        printf("Region %d: Base = 0x%x, Length = 0x%x, Type = %d, ACPI = %d\n", i, (uint32_t)entry.base, (uint32_t)entry.length, type, entry.acpi);
    }
}
