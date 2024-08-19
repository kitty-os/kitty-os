#pragma once

constexpr uint16_t IMAGE_FILE_RELOCS_STRIPPED       = 0x0001;  // Image only, Windows CE, and Microsoft Windows NT and later. Indicates no base relocations; must load at preferred base address.
constexpr uint16_t IMAGE_FILE_EXECUTABLE_IMAGE      = 0x0002;  // Image only. Indicates the image file is valid and can be run.
constexpr uint16_t IMAGE_FILE_LINE_NUMS_STRIPPED    = 0x0004;  // COFF line numbers have been removed. Deprecated; should be zero.
constexpr uint16_t IMAGE_FILE_LOCAL_SYMS_STRIPPED   = 0x0008;  // COFF symbol table entries for local symbols removed. Deprecated; should be zero.
constexpr uint16_t IMAGE_FILE_AGGRESSIVE_WS_TRIM    = 0x0010;  // Obsolete. Aggressively trim working set. Deprecated; should be zero.
constexpr uint16_t IMAGE_FILE_LARGE_ADDRESS_AWARE   = 0x0020;  // Application can handle > 2-GB addresses.
constexpr uint16_t IMAGE_FILE_RESERVED              = 0x0040;  // Reserved for future use.
constexpr uint16_t IMAGE_FILE_BYTES_REVERSED_LO     = 0x0080;  // Little endian. Deprecated; should be zero.
constexpr uint16_t IMAGE_FILE_32BIT_MACHINE         = 0x0100;  // Machine is based on a 32-bit-word architecture.
constexpr uint16_t IMAGE_FILE_DEBUG_STRIPPED        = 0x0200;  // Debugging information is removed from the image file.
constexpr uint16_t IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP = 0x0400;  // Fully load and copy to swap file if on removable media.
constexpr uint16_t IMAGE_FILE_NET_RUN_FROM_SWAP     = 0x0800;  // Fully load and copy to swap file if on network media.
constexpr uint16_t IMAGE_FILE_SYSTEM                = 0x1000;  // The image file is a system file, not a user program.
constexpr uint16_t IMAGE_FILE_DLL                   = 0x2000;  // The image file is a dynamic-link library (DLL).
constexpr uint16_t IMAGE_FILE_UP_SYSTEM_ONLY        = 0x4000;  // The file should be run only on a uniprocessor machine.
constexpr uint16_t IMAGE_FILE_BYTES_REVERSED_HI     = 0x8000;  // Big endian. Deprecated; should be zero.
