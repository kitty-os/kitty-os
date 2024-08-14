#pragma once

#include <cstdint>

struct FAT32Header
{
    // BIOS Parameter Block (BPB)
    uint8_t  jmpBoot[3];            // Jump instruction to boot code
    uint8_t  OEMName[8];            // OEM Name (Not used in the boot process)
    uint16_t bytesPerSector;        // Bytes per sector
    uint8_t  sectorsPerCluster;     // Sectors per cluster
    uint16_t reservedSectorCount;   // Number of reserved sectors
    uint8_t  numFATs;               // Number of File Allocation Tables (FAT)
    uint16_t rootEntryCount;        // Number of root directory entries (0 for FAT32)
    uint16_t totalSectors16;        // Total sectors (if zero, use totalSectors32)
    uint8_t  media;                 // Media descriptor
    uint16_t FATSize16;             // Size of one FAT (if zero, use FATSize32)
    uint16_t sectorsPerTrack;       // Sectors per track (for interrupt 0x13)
    uint16_t numHeads;              // Number of heads (for interrupt 0x13)
    uint32_t hiddenSectors;         // Number of hidden sectors
    uint32_t totalSectors32;        // Total sectors (if totalSectors16 is zero)

    // FAT32 Extended BIOS Parameter Block
    uint32_t FATSize32;             // Size of one FAT in sectors
    uint16_t extFlags;              // Extended flags
    uint16_t FSVersion;             // File system version (usually 0x0000)
    uint32_t rootCluster;           // Cluster number of the root directory start
    uint16_t FSInfo;                // Sector number of FSInfo structure
    uint16_t backupBootSector;      // Sector number of the backup boot sector
    uint8_t  reserved[12];          // Reserved (must be zero)
    uint8_t  driveNumber;           // Logical drive number (for interrupt 0x13)
    uint8_t  reserved1;             // Reserved (used by Windows NT)
    uint8_t  bootSignature;         // Extended boot signature (0x29 if next three fields are valid)
    uint32_t volumeID;              // Volume ID serial number
    uint8_t  volumeLabel[11];       // Volume label (padded with spaces)
    uint8_t  fileSystemType[8];     // File system type (FAT32, padded with spaces)

    // Boot code (remainder of the sector, up to 512 bytes)
    uint8_t  bootCode[420];         // Boot code
    uint16_t bootSignature2;        // Boot sector signature (0xAA55)
} __attribute__((packed));

static_assert(sizeof(FAT32Header) == 512, "FAT32 header isn't 512 bytes.");