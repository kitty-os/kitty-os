//
// Created by Piotr on 26.07.2024.
//

#include <Ke/debug.hpp>
#include "pe32p.hpp"

void PE32PExeGetInformation(PE32PImageInformation* information, void* image)
{
    const IMAGE_DOS_HEADER* dos_header = (const IMAGE_DOS_HEADER*) image;

    // Check if the magic is MZ
    if (dos_header->e_magic != 0x5A4D)
    {
        information->is_valid = false;
        return;
    }

    const IMAGE_FILE_HEADER* file_header = nullptr;
    const IMAGE_OPTIONAL_HEADER64* optional_header = nullptr;
    const IMAGE_SECTION_HEADER* section_header = nullptr;

    const uint8_t* pe_header_start = reinterpret_cast<const uint8_t*>(reinterpret_cast<uintptr_t>(image) + dos_header->e_lfanew);
    const uint32_t* signature = reinterpret_cast<const uint32_t*>(pe_header_start);

    // Check if signature is 'PE\0\0'
    if (*signature != 0x00004550)
    {
        information->is_valid = false;
        return;
    }

    file_header = reinterpret_cast<const IMAGE_FILE_HEADER*>(pe_header_start + sizeof(uint32_t));
    optional_header = reinterpret_cast<const IMAGE_OPTIONAL_HEADER64*>(pe_header_start + sizeof(uint32_t) + sizeof(IMAGE_FILE_HEADER));

    section_header = reinterpret_cast<const IMAGE_SECTION_HEADER*>(reinterpret_cast<uintptr_t>(optional_header) + file_header->SizeOfOptionalHeader);

    information->is_valid = true;
    information->dos_header = dos_header;
    information->file_header = file_header;
    information->optional_header = optional_header;
    information->section_header = section_header;
}
