//
// Created by Piotr on 26.07.2024.
//

#include <Rtl/string.hpp>
#include <Ke/debug.hpp>
#include "pe32p.hpp"

int test(int a, int b)
{
    return (a + b) * b;
}

void PE32PExeResolveImportTable(void* image, PE32PImageInformation* information)
{
    const IMAGE_DOS_HEADER* dosHeader = information->dos_header;
    const IMAGE_FILE_HEADER* fileHeader = information->file_header;
    const IMAGE_OPTIONAL_HEADER64* optionalHeader = information->optional_header;
    const IMAGE_SECTION_HEADER* sectionHeaders = information->section_header;

    // Calculate the address of the import directory
    uint32_t importDirectoryRVA = optionalHeader->DataDirectory[1].VirtualAddress;
    uint32_t importDirectorySize = optionalHeader->DataDirectory[1].Size;

    if (importDirectoryRVA == 0 || importDirectorySize == 0) {
        DbgPrint("No import directory present.\n");
        return;
    }

    // Find the section that contains the import directory
    const IMAGE_SECTION_HEADER* importSection = nullptr;
    for (int i = 0; i < fileHeader->NumberOfSections; ++i) {
        const IMAGE_SECTION_HEADER& section = sectionHeaders[i];
        uint32_t sectionEnd = section.VirtualAddress + section.Misc.VirtualSize;
        if (importDirectoryRVA >= section.VirtualAddress && importDirectoryRVA < sectionEnd) {
            importSection = &section;
            break;
        }
    }

    if (!importSection) {
        DbgPrint("Could not find the section containing the import directory.\n");
        return;
    }

    // Calculate the base address of the image
    uint8_t* baseAddress = static_cast<uint8_t*>(image);

    // Get the file offset for the import directory
    uint32_t importDirectoryOffset = importSection->PointerToRawData + (importDirectoryRVA - importSection->VirtualAddress);

    // Get pointer to import descriptors
    IMAGE_IMPORT_DESCRIPTOR* importDescriptors = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(baseAddress + importDirectoryOffset);

    // Iterate over import descriptors
    for (IMAGE_IMPORT_DESCRIPTOR* descriptor = importDescriptors; descriptor->Name != 0; ++descriptor) {
        // Retrieve the DLL name
        char* dllName = reinterpret_cast<char*>(baseAddress + importSection->PointerToRawData + (descriptor->Name - importSection->VirtualAddress));
        DbgPrint(dllName);
        DbgPrint("\n\n");

        // Get the thunk data
        uint32_t thunkRVA = descriptor->DUMMYUNIONNAME.OriginalFirstThunk ? descriptor->DUMMYUNIONNAME.OriginalFirstThunk : descriptor->FirstThunk;
        uint32_t thunkOffset = importSection->PointerToRawData + (thunkRVA - importSection->VirtualAddress);
        IMAGE_THUNK_DATA* thunkData = reinterpret_cast<IMAGE_THUNK_DATA*>(baseAddress + thunkOffset);

        // Iterate over the thunk data
        for (; thunkData->u1.AddressOfData != 0; ++thunkData) {
            if (thunkData->u1.AddressOfData & 0x80000000) {
                // Import by ordinal
                uint16_t ordinal = static_cast<uint16_t>(thunkData->u1.AddressOfData & 0xFFFF);

            } else {
                // Import by name
                IMAGE_IMPORT_BY_NAME* importByName = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(baseAddress + (thunkData->u1.AddressOfData - importSection->VirtualAddress + importSection->PointerToRawData));

                if (strcmp(dllName, "KERNEL.DLL") == 0 && strcmp(importByName->Name, "KeCalculate") == 0)
                {
                    DbgPrint("Resolving.\n");
                    uint64_t* ptr = (uint64_t*)(thunkData->u1.AddressOfData + (uintptr_t)information->image_base_address);
                    *ptr = (uint64_t)&test;
                }

                DbgPrint(importByName->Name);
                DbgPrintChar('\n');
            }
        }
    }
}

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

