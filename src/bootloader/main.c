#include <efi.h>
#include <efilib.h>

#include "include/bootloader.h"

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS status;
    BOOT_INFO boot_info = {0};

    InitializeLib(ImageHandle, SystemTable);

    status = SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    if(EFI_ERROR(status)) {
        Print(L"NOTE: Couldn't clear the screen... This is awkward.\n");
    }

    Print(L"EggOS Bootloader v%d.%d\n", 1, 0);
    Print(L"=====================\n\n");


    Print(L"Getting system information...\n");
    Print(L"UEFI Version: %d.%d\n",
          SystemTable->Hdr.Revision >> 16,
          SystemTable->Hdr.Revision & 0xFFFF);
    Print(L"Firmware Vendor: %s\n", SystemTable->FirmwareVendor);
    key_wait();

    // load kernel

    Print(L"\nRetrieving memory map...\n");
    status = get_memory_map(&boot_info);
    if (EFI_ERROR(status)) {
        Print(L"Error: Failed to get memory map!\n");
        while(1) {};
        return status;
    }
    key_wait();

    // GOP here, later

    status = SystemTable->BootServices->ExitBootServices(ImageHandle, boot_info.memmap_key);
    if(EFI_ERROR(status)) {
        Print(L"Error: ExitBootServices failed: %r\n", status);
    }
    boot_info.magic = 0x12345678;

    while(1) {};
    // jump to kernel

    return EFI_SUCCESS;
}

EFI_STATUS key_wait() {
    EFI_STATUS status;
    EFI_INPUT_KEY key;

    status = ST->ConOut->OutputString(ST->ConOut, L"Press any key to continue...");
    if (EFI_ERROR(status))
    {
        return status;
    }

    status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(status))
    {
        return status;
    }

    while ((status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key)) == EFI_NOT_READY);

    status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(status))
    {
        return status;
    }

    Print(L"\r\n");

    return status;
}
