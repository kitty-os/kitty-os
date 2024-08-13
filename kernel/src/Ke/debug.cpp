//
// Created by Piotr on 21.07.2024.
//

#include <stb_sprintf.h>
#include <Io/io.hpp>
#include <cstdarg>
#include "debug.hpp"

void DbgPrintChar(const char chr)
{
    outb(DEBUG_PORT, chr);
}

void DbgPrint(const char* msg)
{
    while (*msg != '\0')
    {
        DbgPrintChar(*msg++);
    }
}

void DbgPrintHexadecimal(uint64_t hexadecimal)
{
    // Special case for zero
    if (hexadecimal == 0)
    {
        DbgPrintChar('0');
        return;
    }

    // Determine the number of digits needed
    int shift = 60;  // 60 = (64 - 4 * 1) to start with the highest digit
    while (shift >= 0 && (hexadecimal >> shift) == 0)
    {
        shift -= 4;
    }

    // Print each hexadecimal digit
    for (; shift >= 0; shift -= 4)
    {
        char digit = HEX_DIGITS[(hexadecimal >> shift) & 0xF];
        DbgPrintChar(digit);
    }
}

void DbgPrintf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char buffer[1024] = {0};
    stbsp_vsnprintf(buffer, 1024, fmt, args);
    DbgPrint(buffer);

    va_end(args);
}
