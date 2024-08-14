//
// Created by Piotr on 13.08.2024.
//

#include <stb_sprintf.h>
#include <Ke/debug.hpp>
#include <cstdarg>

extern "C" bool __libc_single_threaded = true;
extern "C" void* stderr = nullptr;

extern "C" int fprintf(void *stream, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char buffer[1024];
    stbsp_vsnprintf(buffer, 1024, fmt, args);
    DbgPrint(buffer);

    va_end(args);
    return 0;
}