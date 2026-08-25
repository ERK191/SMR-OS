@echo off
echo =========================================
echo Compilare SMR-OS v0.0.4 (UEFI 64-bit)
echo =========================================

:: Creare structură foldere în EFI pentru rulare
if not exist EFI\BOOT mkdir EFI\BOOT
if not exist EFI\res\images mkdir EFI\res\images

:: Copiere automată a imaginii tale în folderul final de boot
if exist res\images\wallpaper.bmp (
    xcopy /Y res\images\wallpaper.bmp EFI\res\images\ >nul
    echo [INFO] Wallpaper-ul a fost copiat in folderul de distributie.
) else (
    echo [ATENTIE] Nu s-a gasit imaginea in res\images\wallpaper.bmp!
)

:: Compilare directă cu Clang
clang -target x86_64-unknown-windows -nostdlib -fuse-ld=lld-link -shared -o EFI\BOOT\BOOTX64.EFI src\main.c

if %errorlevel% neq 0 (
    echo [EROARE] Generarea BOOTX64.EFI a esuat!
    pause
    exit /b %errorlevel%
)

echo =========================================
echo [SUCCES] Sistemul si resursele sunt gata in folderul /EFI!
echo =========================================
pause
