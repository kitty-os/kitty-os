//
// Created by Piotr on 09.08.2024.
//

#include <stb_sprintf.h>
#include <Ke/debug.hpp>
#include <Io/cpu.hpp>
#include <cstdarg>

namespace std
{
    void __throw_bad_alloc()
    {
        DbgPrintf("std::bad_alloc(): ??\n");
        IoHaltProcessor();
    }

    void __throw_length_error(const char* message)
    {
        DbgPrintf("std::length_error(): %s\n", message);
        IoHaltProcessor();
    }

    void __throw_bad_array_new_length()
    {
        DbgPrintf("std:bad_array_new_length(): ??\n");
        IoHaltProcessor();
    }

    void __throw_logic_error(const char* message)
    {
        DbgPrintf("std::logic_error(): %s\n", message);
        IoHaltProcessor();
    }

    void __throw_out_of_range_fmt(char const* message, ...)
    {
        va_list args;
        va_start(args, message);
        char buffer[1024];
        stbsp_vsnprintf(buffer, 1024, message, args);
        DbgPrint(buffer);
        va_end(args);
        IoHaltProcessor();
    }

    void __throw_range_error(char const* message)
    {
        DbgPrint(message);
        IoHaltProcessor();
    }
}