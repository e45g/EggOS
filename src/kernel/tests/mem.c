#include "kernel.h"
#include "memory.h"
#include "common.h"
#include <keyboard.h>


static bool is_page_used(uintptr_t addr) {
    extern uint8_t* pmm_bitmap;
    uint32_t page_index = addr / PAGE_SIZE;
    return (pmm_bitmap[page_index / 8] & (1 << (page_index % 8)));
}

int test_pmm_basic_alloc_free() {
    printf("Running test_pmm_basic_alloc_free...\n");
    void* page1 = pmm_alloc_page();
    if (page1 == NULL) {
        PANIC("PMM Test: Failed to allocate page1");
        return -1;
    }
    if (!is_page_used((uintptr_t)page1)) {
        PANIC("PMM Test: Page1 not marked as used");
        return -1;
    }
    printf("  Allocated page1: %p\n", page1);

    void* page2 = pmm_alloc_page();
    if (page2 == NULL) {
        PANIC("PMM Test: Failed to allocate page2");
        return -1;
    }
    if (!is_page_used((uintptr_t)page2)) {
        PANIC("PMM Test: Page2 not marked as used");
        return -1;
    }
    printf("  Allocated page2: %p\n", page2);


    if (page1 == page2) {
        PANIC("PMM Test: page1 and page2 are the same address!");
        return -2;
    }

    uintptr_t p2_addr = (uintptr_t)page2;
    pmm_free_page(page2);
    if (is_page_used(p2_addr)) {
        PANIC("PMM Test: Page2 not marked as free after pmm_free_page");
        return -1;
    }
    printf("  Freed page2\n");

    void* page3 = pmm_alloc_page();
    if (page3 == NULL) {
        PANIC("PMM Test: Failed to allocate page3");
        return -1;
    }
    if (!is_page_used((uintptr_t)page3)) {
        PANIC("PMM Test: Page3 not marked as used");
        return -1;
    }
    printf("  Allocated page3: %p\n", page3);

    if (page3 != page2) {
        PANIC("PMM Test: page3 did not reuse page2's address");
        return -3;
    }

    pmm_free_page(page1);
    pmm_free_page(page3);

    if (is_page_used((uintptr_t)page1) || is_page_used((uintptr_t)page3)) {
        PANIC("PMM Test: Pages not marked as free after final cleanup");
        return -1;
    }

    printf("test_pmm_basic_alloc_free PASSED.\n");
    return 0;
}

int test_vmm_map_unmap() {
    printf("Running test_vmm_map_unmap...\n");
    uint32_t vaddr = 0x5000000;
    uint32_t paddr_target = 0xB8000;
    uint32_t flags = VMM_PRESENT | VMM_RW | VMM_USER;
    char test_char = 'V';

    // 1. Test basic map
    if (map_page(vaddr, paddr_target, flags) != 0) {
        PANIC("VMM Test: map_page failed");
        return -1;
    }
    printf("  Mapped vaddr 0x%x to paddr 0x%x\n", vaddr, paddr_target);


    // 2. Test get_mapping
    uint32_t paddr_mapped = get_mapping(vaddr);
    if ((paddr_mapped & PAGE_MASK) != (paddr_target & PAGE_MASK)) {
        printf("  Mapped paddr: 0x%x, Target paddr: 0x%x\n", paddr_mapped, paddr_target);
        PANIC("VMM Test: get_mapping returned incorrect physical address");
        return -2;
    }
    printf("  get_mapping(0x%x) returned 0x%x (Correct)\n", vaddr, paddr_mapped);

    // 3. Test writing and reading through virtual address
    volatile char* vptr = (volatile char*)vaddr;
    *vptr = test_char;
    printf("  Wrote '%c' to vaddr 0x%x\n", test_char, vaddr);
    // check vga manually

    if (*vptr != test_char) {
        PANIC("VMM Test: Read back incorrect value from virtual address");
        return -3;
    }
    printf("  Read back '%c' from vaddr 0x%x (Correct)\n", *vptr, vaddr);

    // 4. Test unmap
    unmap_page(vaddr);
    printf("  Unmapped vaddr 0x%x\n", vaddr);

    // 5. Test get_mapping after unmap
    paddr_mapped = get_mapping(vaddr);
    if (paddr_mapped != 0) {
        PANIC("VMM Test: get_mapping did not return 0 after unmap");
        return -4;
    } else {
        printf("  get_mapping(0x%x) after unmap returned 0 (Correct)\n", vaddr);
    }


    printf("test_vmm_map_unmap PASSED.\n");
    return 0;
}

// --- Heap Tests ---

int test_heap_basic_malloc_free() {
    printf("Running test_heap_basic_malloc_free...\n");
    size_t size1 = 10;
    size_t size2 = 500;
    size_t size3 = PAGE_SIZE;

    void* ptr1 = malloc(size1);
    if (!ptr1) {
        PANIC("Heap Test: malloc(10) failed");
        return -1;
    }
    printf("  malloc(%d) returned %p\n", size1, ptr1);
    memset(ptr1, 0xAA, size1);

    void* ptr2 = malloc(size2);
    if (!ptr2) {
        free(ptr1);
        PANIC("Heap Test: malloc(500) failed");
        return -1;
    }
    printf("  malloc(%d) returned %p\n", size2, ptr2);
    memset(ptr2, 0xBB, size2);

    void* ptr3 = malloc(size3);
    if (!ptr3) {
        free(ptr1);
        free(ptr2);
        PANIC("Heap Test: malloc(PAGE_SIZE) failed");
        return -1;
    }
    printf("  malloc(%d) returned %p\n", size3, ptr3);
    memset(ptr3, 0xCC, size3);


    if ((uintptr_t)ptr1 + size1 > (uintptr_t)ptr2 || (uintptr_t)ptr2 + size2 > (uintptr_t)ptr3) {
        printf("  ptr1=%p size1=%d ptr2=%p size2=%d ptr3=%p size3=%d\n", ptr1, size1, ptr2, size2, ptr3, size3);
        printf("  WARN: Basic overlap check failed. Review allocator alignment/metadata.\n");
    }


    free(ptr2);
    printf("  Freed ptr2 (%p)\n", ptr2);
    ptr2 = NULL;

    size_t size4 = 400;
    void* ptr4 = malloc(size4);
    if (!ptr4) {
        free(ptr1);
        free(ptr3);
        PANIC("Heap Test: malloc(400) after free failed");
        return -1;
    }
    printf("  malloc(%d) returned %p\n", size4, ptr4);
    memset(ptr4, 0xDD, size4);

    if ((uintptr_t)ptr4 >= (uintptr_t)ptr2 && (uintptr_t)ptr4 < (uintptr_t)ptr2 + size2 + 32) { // Allow some slack for metadata/alignment
        printf("  ptr4 seems to have reused (part of) ptr2's space (Address: %p)\n", ptr4);
    } else {
        printf("  ptr4 (%p) did not reuse ptr2's space (%p). Might be ok.\n", ptr4, ptr2);
    }


    free(ptr1);
    printf("  Freed ptr1 (%p)\n", ptr1);
    free(ptr3);
    printf("  Freed ptr3 (%p)\n", ptr3);
    free(ptr4);
    printf("  Freed ptr4 (%p)\n", ptr4);

    // Test freeing NULL
    free(NULL);
    printf("  free(NULL) executed (should not crash)\n");

    printf("test_heap_basic_malloc_free PASSED.\n");
    return 0;
}

void run_memory_tests() {
    printf("\n--- Running Memory Tests ---\n");
    test_pmm_basic_alloc_free();
    getchar();
    test_vmm_map_unmap();
    getchar();
    test_heap_basic_malloc_free();
    printf("--- Memory Tests Finished ---\n\n");
}
