#include "kernel.h"
#include <memory.h>

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

extern void enable_paging(void);

void vmm_init() {
    for(int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
        first_page_table[i] = (i * 0x1000) | 3;
    }
    page_directory[0] = ((uintptr_t) first_page_table) | PRESENT|READ_WRITE;
    page_directory[1023] = (uintptr_t)page_directory | PRESENT|READ_WRITE;

    enable_paging();
}

void map_page(uint32_t vaddr, uint32_t paddr, uint32_t flags) {
    uint32_t pd_index = vaddr >> 22;
    uint32_t pt_index = (vaddr >> 12) & 0x3FF;

    uint32_t *page_table = (uint32_t *) (RECURSIVE_BASE + pd_index * PAGE_SIZE);

    if(!(PD_VIRT[pd_index] & 0x1)) {
        void *new_table = alloc_page();
        memset(new_table, 0, PAGE_SIZE);
        PD_VIRT[pd_index] = ((uintptr_t)new_table) | flags | PRESENT;
    }

    page_table = (uint32_t *)(RECURSIVE_BASE + (pd_index * PAGE_SIZE));

    page_table[pt_index] = (paddr & PAGE_MASK) | flags | PRESENT;

    asm volatile ("invlpg (%0)" : : "r"(vaddr) : "memory");
}

void unmap_page(uint32_t vaddr) {
    uint32_t pd_index = vaddr >> 22;
    uint32_t pt_index = (vaddr >> 12) & 0x3FF;

    uint32_t *page_table = (uint32_t *) (RECURSIVE_BASE + pd_index * PAGE_SIZE);

    if(!(PD_VIRT[pd_index] & 0x1)) return;
    if(!(page_table[pt_index] & 0x1)) return;

    page_table[pt_index] = 0x0;
    asm volatile ("invlpg (%0)" : : "r"(vaddr) : "memory");
}

uint32_t get_mapping(uint32_t vaddr) {
    uint32_t pd_index = vaddr >> 22;
    uint32_t pt_index = (vaddr >> 12) & 0x3FF;
    uint32_t offset = vaddr & 0xFFF;

    const uint32_t *page_table = (uint32_t*)(RECURSIVE_BASE + (pd_index * PAGE_SIZE));

    if(!(PD_VIRT[pd_index] & 0x1)) return 0;
    if(!(page_table[pt_index] & 0x1)) return 0;

    uint32_t paddr = (page_table[pt_index] & PAGE_MASK) + offset;
    return paddr;
}

void *alloc_page() {
    void *page = pmm_alloc_page();
    if(page == NULL) {
        PANIC("VMM - out of memory!!");
        return NULL;
    }
    return page;
}

void free_page(void *page) {
    pmm_free_page(page);
}
