#include "common.h"
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
    size_t grow_size = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    grow_size *= PAGE_SIZE;

    for(size_t i = 0; i < grow_size; i += PAGE_SIZE) {
        uintptr_t paddr = (uintptr_t) pmm_alloc_page();
        if(paddr == 0) return -1;
        map_page(heap_end + i, paddr, VMM_RW | VMM_USER);
    }

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
    if(size == 0) return NULL;
    size_t total_size = ALIGN_UP(size + sizeof(heap_node_t), 8);

    heap_node_t *current = heap_head;
    while(current != NULL) {
        if(current->is_free == true && current->size >= (total_size - sizeof(heap_node_t))) {
            if(current->size > total_size + MIN_SPLIT) {
                printf("[HEAP] Splitting : node_size: %lu | requested size: %lu\n", current->size, total_size);

                heap_node_t *new_node = (heap_node_t*)((uintptr_t)current + total_size);
                new_node->size = current->size - total_size;
                new_node->is_free = true;
                new_node->next = current->next;
                new_node->prev = current;
                if(current->next) current->next->prev = new_node;
                if (current == heap_tail) heap_tail = new_node;

                current->size = total_size - sizeof(heap_node_t);
                current->next = new_node;
            }
            // printf("[HEAP] Node found : size: %lu | address: %p\n", current->size, current);
            current->is_free = false;
            return (void*)((uintptr_t)current+sizeof(heap_node_t));
        }
        current = current->next;
    }

    // printf("[HEAP] Growing heap : size: %lu\n", total_size);
    if(heap_grow(total_size) == 0) {
        return malloc(size);
    }

    return NULL;
}

void free(void *ptr) {
    if(ptr == NULL) return;
    heap_node_t *node = (heap_node_t*) ((uintptr_t)ptr - sizeof(heap_node_t));
    if(node == NULL) return;

    node->is_free = true;

    if(node->prev && node->prev->is_free) {
        node->prev->size += node->size + sizeof(heap_node_t);
        node->prev->next = node->next;
        if(node->next) node->next->prev = node->prev;
        node = node->prev;
    }
    if(node->next && node->next->is_free) {
        node->size += node->next->size + sizeof(heap_node_t);
        heap_node_t *yank_me = node->next;
        node->next = yank_me->next;
        if(yank_me->next) yank_me->next->prev = node;
    }
    if(heap_tail == node && node->size >= PAGE_SIZE) {
        size_t total_size = node->size + sizeof(heap_node_t);
        size_t pages = (total_size + PAGE_SIZE - 1) / PAGE_SIZE;

        for(size_t k = 0; k < pages; k++) {
            uintptr_t vaddr = heap_end - PAGE_SIZE*(k+1);
            uintptr_t paddr = get_mapping(vaddr);
            unmap_page(vaddr);
            pmm_free_page((void*)paddr);
        }
        heap_end -= PAGE_SIZE * pages;
        node->size = total_size - PAGE_SIZE * pages - sizeof(heap_node_t);
        if(node->size == 0) {
            heap_tail = node->prev;
            if(heap_tail != NULL) heap_tail->next = NULL;
            else heap_head = NULL;
        }
    }
}

void heap_print() {
    const heap_node_t *current = heap_head;
    size_t i = 0;
    while(current != NULL) {
        printf("[HEAP] Block %u: free: %d | size: %lu | total_size: %lu | addr: %p\n", i, current->is_free, current->size, current->size + sizeof(heap_node_t), current);
        current = current -> next;
        i++;
    }
    printf("\n");
}
