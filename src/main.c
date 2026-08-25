// ============================================================================
// SMR-OS v0.0.4 - WALLPAPER ȘI TASKBAR GRAFIC (64-bit UEFI)
// ============================================================================

#pragma comment(linker, "/subsystem:efi_application")
#pragma comment(linker, "/entry:EfiMain")

unsigned long long typedef UINTN;
unsigned int typedef UINT32;
short typedef CHAR16;

typedef struct {
    UINT32 Data1; short Data2; short Data3; char Data4[8];
} EFI_GUID;

// Definiții GUID corectate pentru a elimina orice warning la compilare
#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID {0x41351778, 0x42e5, 0x4cd2, {0x8e, 0x3a, 0x9e, 0xcc, 0x54, 0x57, 0x73, 0xc1}}
#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID {0x0964e5b2, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

// Structuri video GOP
typedef struct {
    UINT32 Version; UINT32 HorizontalResolution; UINT32 VerticalResolution;
    int PixelFormat; UINT32 PixelInformation; UINT32 PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct {
    UINT32 MaxMode; UINT32 Mode; EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    UINTN SizeOfInfo; unsigned long long FrameBufferBase; UINTN FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct { void* QueryMode; void* SetMode; void* Blt; EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode; } EFI_GRAPHICS_OUTPUT_PROTOCOL;

// Structuri fișiere UEFI
struct _EFI_FILE_PROTOCOL;
typedef unsigned long long (*EFI_FILE_OPEN)(struct _EFI_FILE_PROTOCOL *This, struct _EFI_FILE_PROTOCOL **NewHandle, CHAR16 *FileName, unsigned long long OpenMode, unsigned long long Attributes);
typedef unsigned long long (*EFI_FILE_CLOSE)(struct _EFI_FILE_PROTOCOL *This);
typedef unsigned long long (*EFI_FILE_READ)(struct _EFI_FILE_PROTOCOL *This, UINTN *BufferSize, void *Buffer);

typedef struct _EFI_FILE_PROTOCOL { unsigned long long Revision; EFI_FILE_OPEN Open; EFI_FILE_CLOSE Close; void* Delete; EFI_FILE_READ Read; } EFI_FILE_PROTOCOL;
typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL { unsigned long long Revision; unsigned long long (*OpenVolume)(struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This, EFI_FILE_PROTOCOL **Root); } EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

// Structuri de sistem standard
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef unsigned long long (*EFI_TEXT_CLEAR_SCREEN)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
typedef unsigned long long (*EFI_TEXT_STRING)(struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This, CHAR16 *String);
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL { void* Reset; EFI_TEXT_STRING OutputString; void* TestString; void* QueryMode; void* SetMode; void* SetAttribute; EFI_TEXT_CLEAR_SCREEN ClearScreen; } EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef unsigned long long (*EFI_LOCATE_PROTOCOL)(EFI_GUID *Protocol, void *Registration, void **Interface);

typedef struct {
    char Header; void *AllocPages; void *FreePages; void *GetMap; void *AllocPool; void *FreePool;
    void *Evnt; void *Timer; void *Wait; void *CloseEvnt; void *Signl; void *OpenProt; void *CloseProt; void *HndlProt;
    void *ProtPerHndl; void *RegProt; void *LocHndlBuf; void *LocRegProt; void *InstMult; void *UninstMult;
    void *CRC; void *CopyMem; void *SetMem; void *CreateEvntEx;
    EFI_LOCATE_PROTOCOL LocateProtocol;
} EFI_BOOT_SERVICES;

typedef struct { char Header; CHAR16 *Vendor; UINT32 Revision; void *InHndl; void *In; void *OutHndl; EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut; void *ErrHndl; void *Err; void *Runtime; EFI_BOOT_SERVICES *BootServices; } EFI_SYSTEM_TABLE;

// Header-ul standard al unui fișier BMP
#pragma pack(push, 1)
typedef struct {
    unsigned short Type; UINT32 Size; unsigned short Reserved1; unsigned short Reserved2; UINT32 OffBits;
    UINT32 SizeHeader; int Width; int Height; unsigned short Planes; unsigned short BitCount;
} BMP_HEADER;
#pragma pack(pop)

// Buffer temporar în RAM unde citim imaginea (16 Megabytes maxim)
unsigned char fileBuffer[16 * 1024 * 1024];

// ============================================================================
// FUNCȚIE: DESENARE TASKBAR (Bara de Start)
// ============================================================================
void DrawTaskbar(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop) {
    UINT32 *fb = (UINT32*)Gop->Mode->FrameBufferBase;
    UINT32 screenW = Gop->Mode->Info->HorizontalResolution;
    UINT32 screenH = Gop->Mode->Info->VerticalResolution;
    UINT32 scanline = Gop->Mode->Info->PixelsPerScanLine;

    UINT32 taskbarHeight = 45;        // Înălțimea bării în pixeli în partea de jos
    UINT32 taskbarColor = 0x000B121C;  // Nuanță închisă de albastru/negru, asortată cu logo-ul tau

    for (UINT32 y = screenH - taskbarHeight; y < screenH; y++) {
        for (UINT32 x = 0; x < screenW; x++) {
            fb[y * scanline + x] = taskbarColor;
        }
    }
}

// ============================================================================
// PUNCTUL DE INTRARE ÎN SISTEMUL DE OPERARE
// ============================================================================
unsigned long long EfiMain(void *ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, (short*)L"SMR-OS: Incarcare interfata grafica...\r\n");

    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = 0;
    SystemTable->BootServices->LocateProtocol(&gopGuid, 0, (void**)&gop);

    EFI_GUID fileSystemGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fileSystem = 0;
    SystemTable->BootServices->LocateProtocol(&fileSystemGuid, 0, (void**)&fileSystem);

    if (gop && fileSystem) {
        EFI_FILE_PROTOCOL *root = 0;
        fileSystem->OpenVolume(fileSystem, &root);

        if (root) {
            EFI_FILE_PROTOCOL *wallpaperFile = 0;
            // Deschiderea imaginii din folderul organizat dedicat res\images
            unsigned long long status = root->Open(root, &wallpaperFile, (CHAR16*)L"res\\images\\wallpaper.bmp", 1, 0);

            if (status == 0 && wallpaperFile) {
                UINTN fileSize = sizeof(fileBuffer);
                wallpaperFile->Read(wallpaperFile, &fileSize, fileBuffer);
                wallpaperFile->Close(wallpaperFile);

                BMP_HEADER *bmp = (BMP_HEADER*)fileBuffer;
                unsigned char *pixelData = fileBuffer + bmp->OffBits;
                UINT32 *fb = (UINT32*)gop->Mode->FrameBufferBase;
                
                UINT32 screenW = gop->Mode->Info->HorizontalResolution;
                UINT32 screenH = gop->Mode->Info->VerticalResolution;
                UINT32 scanline = gop->Mode->Info->PixelsPerScanLine;

                // 1. Randarea Wallpaper-ului complet pe ecran
                for (int y = 0; y < bmp->Height && y < (int)screenH; y++) {
                    for (int x = 0; x < bmp->Width && x < (int)screenW; x++) {
                        int bmpY = bmp->Height - 1 - y;
                        int bmpIdx = (bmpY * bmp->Width + x) * (bmp->BitCount / 8);
                        
                        unsigned char b = pixelData[bmpIdx];
                        unsigned char g = pixelData[bmpIdx + 1];
                        unsigned char r = pixelData[bmpIdx + 2];

                        UINT32 pixelColor = (r << 16) | (g << 8) | b;
                        fb[y * scanline + x] = pixelColor;
                    }
                }

                // 2. Randarea Taskbar-ului deasupra imaginii de fundal
                DrawTaskbar(gop);

            } else {
                SystemTable->ConOut->OutputString(SystemTable->ConOut, (short*)L"Eroare: Nu am gasit 'res\\images\\wallpaper.bmp'!\r\n");
            }
        }
    }

    while (1) { __asm__("hlt"); }
    return 0;
}
