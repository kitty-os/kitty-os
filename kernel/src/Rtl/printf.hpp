#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdarg>

void RtlGenericPrintf(
        void (*putc_function)(char),
        const char* fmt,
        va_list args
);

void RtlGenericPutInt(
        void (*putc_function)(char),
        int integer
);

void RtlGenericPutString(
        void (*putc_function)(char),
        const char* string
);

void RtlGenericPutHexadecimal(
        void (*putc_function)(char),
        uint64_t hexadecimal
);