#include <keyboard.h>
#include <memory.h>
#include <common.h>

void print_memory_map() {
    const uint16_t size = *(uint16_t *)(MEMORY_MAP_ADDR);  // Read the size
    const memory_map_entry_t *map = (memory_map_entry_t *)(MEMORY_MAP_ADDR + 2);  // Start after size field

    printf("Memory Map Size: %d\n", size);

    int entries = size;  // Calculate the number of entries

    for (int i = 0; i < entries; i++) {
        memory_map_entry_t entry = map[i];
        printf("Region %d: Base = 0x%x, Length = 0x%x, Type = %d, ACPI = %d\n",
               i, (uint32_t)entry.base, (uint32_t)entry.length, entry.type, entry.acpi);
    }
}
