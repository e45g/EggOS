#include <efi.h>
#include <efilib.h>

#include "include/bootloader.h"

EFI_STATUS get_memory_map(BOOT_INFO *boot_info) {
    EFI_STATUS status;

    UINTN memory_map_size = 0;
    EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
    UINTN map_key;
    UINTN descriptor_size;
    UINT32 descriptor_version;

    status = gBS->GetMemoryMap(
                 &memory_map_size,
                 memory_map,
                 &map_key,
                 &descriptor_size,
                 &descriptor_version);

    if(status != EFI_BUFFER_TOO_SMALL) {
        Print(L"Error: Unexpected result from GetMemoryMap: %r\n", status);
        return status;
    }

    memory_map_size += 2 * descriptor_size;
    status = gBS->AllocatePool(EfiLoaderData, memory_map_size, (VOID*)&memory_map);

    if (EFI_ERROR(status)) {
        Print(L"Error: Failed to allocate memory for memory map: %r\n", status);
        return status;
    }

    status = gBS->GetMemoryMap(
                 &memory_map_size,
                 memory_map,
                 &map_key,
                 &descriptor_size,
                 &descriptor_version);

    if (EFI_ERROR(status)) {
        Print(L"Error: Failed to get memory map: %r\n", status);
        gBS->FreePool(memory_map);
        return status;
    }

    boot_info->memmap_count = (UINT32)memory_map_size / descriptor_size;
    boot_info->memory_map = (EFI_PHYSICAL_ADDRESS)(UINTN)memory_map;
    boot_info->memmap_key = map_key;

    Print(L"Memory Map Retrieved Successfully!\n");


    return EFI_SUCCESS;
}
