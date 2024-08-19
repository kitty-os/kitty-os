//
// Created by Piotr on 26.07.2024.
//

#include <Rtl/string.hpp>
#include <Ke/debug.hpp>
#include "pe.hpp"

void PEParseImage(void* image_pointer, PEImageInformation* image_information)
{
    /*
    image_information->image_base_address = image_pointer;
    image_information->is_valid = false;

    image_information->dos_header = (const IMAGE_DOS_HEADER*) image_pointer;

    if (image_information->dos_header->e_magic != 0x5A4D)
    {
        return; // invalid dos header.
    }

    const uint8_t* pe_header_start = reinterpret_cast<const uint8_t*>(reinterpret_cast<uintptr_t>(image_pointer) + image_information->dos_header->e_lfanew);
    const uint32_t* signature = reinterpret_cast<const uint32_t*>(pe_header_start);
    if (*signature != 0x00004550)
    {
        return; // invalid file header.
    }
    IMAGE_FILE_HEADER* file_header = reinterpret_cast<const IMAGE_FILE_HEADER*>(pe_header_start + sizeof(uint32_t));
    IMAGE_OPTIONAL_HEADER64* optional_header = reinterpret_cast<const IMAGE_OPTIONAL_HEADER64*>(pe_header_start + sizeof(uint32_t) + sizeof(IMAGE_FILE_HEADER));
    IMAGE_SECTION_HEADER* section_header = reinterpret_cast<const IMAGE_SECTION_HEADER*>(reinterpret_cast<uintptr_t>(optional_header) + file_header->SizeOfOptionalHeader);

    image_information->optional_header = optional_header;
    image_information->file_header = file_header;
    image_information->section_header = section_header;
     */
}

void PEIsMachineType(PEImageInformation* image_information, uint16_t machine_type)
{

}