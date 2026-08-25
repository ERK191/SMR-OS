### 💻 SMR-OS

A 100% open-source, fast, lightweight, and bloatware-free 64-bit Operating System built from scratch for modern UEFI hardware. 

### 🚀 Project Overview

SMR-OS is a hobby operating system designed to run directly on bare-metal hardware. Instead of relying on legacy BIOS, it boots natively via modern **64-bit UEFI (Unified Extensible Firmware Interface)**, putting the processor directly into Long Mode. 

### 🌟 Key Features

* **Pure 64-Bit Architecture**: Runs natively in x86_64 Long Mode.
* **Modern Booting**: UEFI-compliant BOOTX64.EFI entry point.
* **Zero Bloatware**: Every single byte of code is written with performance and lightness in mind.
* **Open Source**: Community-driven and fully transparent.

### 📁 Repository Structure

text

├── .github/          # GitHub configuration files
├── EFI/              # UEFI Bootloader deployment folder
│   └── BOOT/
│       └── BOOTX64.EFI  # Main OS binary compiled from C/Assembly
├── src/              # Core source code
│   └── main.c        # OS entry point (EfiMain)
└── README.md         # Project documentation

Folosește codul cu precauție.

### 🛠️ Tech Stack & Requirements

To compile and play with SMR-OS, you will need: 

* **Language**: C & x86_64 Assembly (NASM)
* **Compiler**: Clang (with x86_64-unknown-windows-gnu target) or MSVC
* **Testing**: QEMU Emulator (for fast testing) or a physical USB drive (FAT32 formatted)

### 🚧 Current Status

We are currently in the **Pre-Alpha (Bootloader) stage**. The OS successfully boots on real UEFI hardware, clears the screen, and takes complete control of the processor. Next milestones include setting up the Graphics Output Protocol (GOP) for visual elements and a custom memory manager.
