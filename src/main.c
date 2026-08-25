// SMR-OS v0.0.1 - Initial UEFI Bootloader
unsigned long long typedef UINTN;
short typedef CHAR16;

struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef unsigned long long (*EFI_TEXT_CLEAR_SCREEN)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
typedef unsigned long long (*EFI_TEXT_STRING)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    void* Reset;
    EFI_TEXT_STRING OutputString;
    void* TestString;
    void* QueryMode;
    void* SetMode;
    void* SetAttribute;
    EFI_TEXT_CLEAR_SCREEN ClearScreen;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct {
    char Header;
    CHAR16 *FirmwareVendor;
    unsigned int FirmwareRevision;
    void *ConsoleInHandle;
    void *ConIn;
    void *ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut; 
} EFI_SYSTEM_TABLE;

unsigned long long EfiMain(void *ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"=== SMR-OS v0.0.1 ===\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Booting SMR Operating System...\r\n");

    while (1) {
        __asm__("hlt");
    }
    return 0;
}
