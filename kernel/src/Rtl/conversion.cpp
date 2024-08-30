//
// Created by Piotr on 09.08.2024.
//

#include "conversion.hpp"


const char* RtlInterpretDataSizeAsString(DataSize size)
{
    switch (size)
    {
        case BYTE:        return "Byte";
        case KILOBYTE:    return "Kilobyte";
        case MEGABYTE:    return "Megabyte";
        case GIGABYTE:    return "Gigabyte";
        case TERABYTE:    return "Terabyte";
        case PETABYTE:    return "Petabyte";
        case KIBIBYTE:    return "Kibibyte";
        case MEBIBYTE:    return "Mebibyte";
        case GIBIBYTE:    return "Gibibyte";
        case TEBIBYTE:    return "Tebibyte";
        case PEBIBYTE:    return "Pebibyte";
        default:          return "Unknown Data Size";
    }
}

const wchar_t* RtlInterpretDataSizeAsWideString(DataSize size)
{
    switch (size)
    {
        case BYTE:        return L"Byte";
        case KILOBYTE:    return L"Kilobyte";
        case MEGABYTE:    return L"Megabyte";
        case GIGABYTE:    return L"Gigabyte";
        case TERABYTE:    return L"Terabyte";
        case PETABYTE:    return L"Petabyte";
        case KIBIBYTE:    return L"Kibibyte";
        case MEBIBYTE:    return L"Mebibyte";
        case GIBIBYTE:    return L"Gibibyte";
        case TEBIBYTE:    return L"Tebibyte";
        case PEBIBYTE:    return L"Pebibyte";
        default:          return L"Unknown Data Size";
    }
}

const char* RtlInterpretDataSizeAsStringShortForm(DataSize size)
{
    switch (size)
    {
        case BYTE:        return "B";
        case KILOBYTE:    return "KB";
        case MEGABYTE:    return "MB";
        case GIGABYTE:    return "GB";
        case TERABYTE:    return "TB";
        case PETABYTE:    return "PB";
        case KIBIBYTE:    return "KiB";
        case MEBIBYTE:    return "MiB";
        case GIBIBYTE:    return "GiB";
        case TEBIBYTE:    return "TiB";
        case PEBIBYTE:    return "PiB";
        default:          return "Unknown";
    }
}

const wchar_t* RtlInterpretDataSizeAsWideStringShortForm(DataSize size)
{
    switch (size)
    {
        case BYTE:        return L"B";
        case KILOBYTE:    return L"KB";
        case MEGABYTE:    return L"MB";
        case GIGABYTE:    return L"GB";
        case TERABYTE:    return L"TB";
        case PETABYTE:    return L"PB";
        case KIBIBYTE:    return L"KiB";
        case MEBIBYTE:    return L"MiB";
        case GIBIBYTE:    return L"GiB";
        case TEBIBYTE:    return L"TiB";
        case PEBIBYTE:    return L"PiB";
        default:          return L"Unknown";
    }
}

double RtlConvertDataSize(uint64_t data_size, DataSize input_datasize, DataSize output_datasize)
{
    if (input_datasize == 0 || output_datasize == 0)
    {
        // Handle error: division by zero or invalid data size unit
        return 0.0;
    }

    // Convert input data size to bytes
    double bytes = static_cast<double>(data_size) * static_cast<double>(input_datasize);

    // Convert bytes to the output data size unit
    double converted_size = bytes / static_cast<double>(output_datasize);

    return converted_size;
}