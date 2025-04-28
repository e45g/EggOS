#include "kernel.h"
#include "utils.h"
#include <memory.h>

uintptr_t heap_start;
uintptr_t heap_end;

heap_node_t *heap_head;
heap_node_t *heap_tail;

void heap_init(void) {
    heap_start = kernel_end + 0x1000;
    heap_end = heap_start;

    heap_head = NULL;
    heap_tail = NULL;
}

int heap_grow(size_t size) {
    size_t grow_size = PAGE_SIZE;
    while(grow_size <= size) {
        grow_size += PAGE_SIZE;
    }

    for(size_t i = 0; i < grow_size; i += PAGE_SIZE) {
        uintptr_t paddr = (uintptr_t) pmm_alloc_page();
        if(paddr == 0) return -1;
        map_page(heap_end + i, paddr, VMM_RW | VMM_USER);
    }

    // TODO : If too big, split

    heap_node_t *node = (heap_node_t*)heap_end;
    node->size = grow_size - sizeof(heap_node_t);
    node->is_free = true;
    node->next = NULL;
    node->prev = heap_tail;

    if(heap_tail != NULL) heap_tail->next = node;
    if(heap_head == NULL) heap_head = node;

    heap_tail = node;

    heap_end += grow_size;
    return 0;
}

void *malloc(size_t size) {
    size_t total_size = ALIGN_UP(size + sizeof(heap_node_t), 8);

    heap_node_t *current = heap_head;
    while(current != NULL) {
        if(current->is_free == true && current->size >= (total_size - sizeof(heap_node_t))) {

            // TODO : Split if too big;
            printf("Heap node found. Address: %p; Size: %lu\n", &current, current->size);
            current->is_free = false;
            return (void*)((uintptr_t)current+sizeof(heap_node_t));
        }
        current = current->next;
    }

    printf("Heap node not found. Growing heap. Size: %lu\n", total_size);
    if(heap_grow(total_size) == 0) {
        return malloc(size);
    }


    return NULL;
}
