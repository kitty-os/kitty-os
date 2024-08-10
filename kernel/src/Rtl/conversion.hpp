#pragma once

#include <cstdint>
#include <wchar.h>

enum DataSize
{
    BYTE      = 1ULL,
    KILOBYTE  = 1000ULL,
    MEGABYTE  = 1000000ULL,
    GIGABYTE  = 1000000000ULL,
    TERABYTE  = 1000000000000ULL,
    PETABYTE  = 1000000000000000ULL,

    KIBIBYTE = 1024ULL,
    MEBIBYTE = 1024ULL * 1024ULL,
    GIBIBYTE = 1024ULL * 1024ULL * 1024ULL,
    TEBIBYTE = 1024ULL * 1024ULL * 1024ULL * 1024ULL,
    PEBIBYTE = 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL,
};

const char* RtlInterpretDataSizeAsString(DataSize size);
const wchar_t* RtlInterpretDataSizeAsWideString(DataSize size);
const char* RtlInterpretDataSizeAsStringShortForm(DataSize size);
const wchar_t* RtlInterpretDataSizeAsWideStringShortForm(DataSize size);

double RtlConvertDataSize(uint64_t data_size, DataSize input_datasize, DataSize output_datasize);