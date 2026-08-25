@echo off
echo =========================================
echo Compilare SMR-OS v0.0.1 (UEFI 64-bit)
echo =========================================

if not exist EFI\BOOT mkdir EFI\BOOT

clang -target x86_64-unknown-windows -nostdlib -fuse-ld=lld-link -shared -o EFI\BOOT\BOOTX64.EFI src\main.c

if %errorlevel% neq 0 (
    echo [EROARE] Generarea BOOTX64.EFI a esuat!
    pause
    exit /b %errorlevel%
)

echo =========================================
echo [SUCCES] Fisierul EFI/BOOT/BOOTX64.EFI a fost generat!
echo =========================================
pause
