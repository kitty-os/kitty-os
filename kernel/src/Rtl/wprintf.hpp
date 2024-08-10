#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdarg>
#include <wchar.h>

void RtlGenericWprintf(
        void (*wputc_function)(wchar_t),
        const wchar_t* format,
        va_list args
);

void RtlGenericWPutInt(
        void (*wputc_function)(wchar_t),
        int integer
);

void RtlGenericWPutString(
        void (*wputc_function)(wchar_t),
        const char* string
);

void RtlGenericWPutWString(
        void (*wputc_function)(wchar_t),
        const wchar_t* wstring
);

void RtlGenericWPutHexadecimal(
        void (*wputc_function)(wchar_t),
        uint64_t hexadecimal
);

void RtlGenericWPutDouble(
        void (*wputc_function)(wchar_t),
        double value
);