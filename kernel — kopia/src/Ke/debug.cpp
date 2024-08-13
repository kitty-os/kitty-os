//
// Created by Piotr on 21.07.2024.
//

#include <Rtl/wprintf.hpp>
#include <Rtl/printf.hpp>
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
    RtlGenericPrintf(DbgPrintChar, fmt, args);
    va_end(args);
}

void DbgPrintWchar(const wchar_t wchr)
{
    char utf8[4];

    std::size_t len = wcrtomb(utf8, wchr, nullptr);

    if (len == static_cast<std::size_t>(-1))
        return;

    for (std::size_t i = 0; i < len; ++i)
        outb(DEBUG_PORT, static_cast<uint8_t>(utf8[i]));
}

void DbgWprintf(const wchar_t* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    RtlGenericWprintf(DbgPrintWchar ,fmt, args);
}