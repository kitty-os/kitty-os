#include <Rtl/spinlock.hpp>
#include <Ke/debug.hpp>
#include <cstddef>
#include <cstdint>
#include "printf.hpp"

void RtlGenericPutHexadecimal(void (*putc_function)(char), uint64_t hexadecimal);

void RtlGenericPrintf(
        void (*putc_function)(char), const char* fmt, va_list args
)
{
    static Spinlock sl;

    sl.lock();
    int temp_int;
    unsigned long temp_ulong;
    unsigned long long temp_ullong;
    char temp_char;
    char* temp_string;

    for (const char* p = fmt; *p != '\0'; ++p)
    {
        if (*p == '%')
        {
            ++p;
            switch (*p)
            {
                case 'd' :
                {
                    temp_int = va_arg(args, int);
                    RtlGenericPutInt(putc_function, temp_int);
                    break;
                }
                case 'c' :
                {
                    temp_char = static_cast<char>(va_arg(args, int));
                    putc_function(temp_char);
                    break;
                }
                case 's' :
                {
                    temp_string = va_arg(args, char*);
                    RtlGenericPutString(putc_function, temp_string);
                    break;
                }
                case 'x' :
                {
                    temp_int = va_arg(args, int);
                    RtlGenericPutHexadecimal(putc_function, temp_int);
                    break;
                }
                case 'l' :
                {
                    ++p;
                    if (*p == 'l' && *(p+1) == 'x')
                    {
                        p++; // Move past 'x'
                        temp_ullong = va_arg(args, unsigned long long);
                        RtlGenericPutHexadecimal(putc_function, temp_ullong);
                    }
                    else if (*p == 'x')
                    {
                        temp_ulong = va_arg(args, unsigned long);
                        RtlGenericPutHexadecimal(putc_function, temp_ulong);
                    }
                    break;
                }
            }
        }
        else
        {
            putc_function(*p);
        }
    }
    sl.unlock();
}

void RtlGenericPutInt(void (*putc_function)(char), int integer) {
    if (integer == 0)
    {
        putc_function('0');
        return;
    }

    if (integer < 0)
    {
        putc_function('-');
        integer = -integer;
    }

    char buffer[10];
    int index = 0;

    while (integer > 0)
    {
        buffer[index++] = '0' + (integer % 10);
        integer /= 10;
    }

    while (index > 0)
    {
        putc_function(buffer[--index]);
    }
}

void RtlGenericPutString(void (*putc_function)(char), const char* string)
{
    while (*string != '\0')
    {
        putc_function(*string++);
    }
}

void RtlGenericPutHexadecimal(void (*putc_function)(char), uint64_t hexadecimal)
{
    wchar_t buffer[16];  // 16 digits for 64-bit hexadecimal
    int pos = 0;

    // Process each nibble (4 bits) from the highest to the lowest
    for (int i = 60; i >= 0; i -= 4)
    {
        int digit = (hexadecimal >> i) & 0xF;  // Extract 4 bits
        buffer[pos++] = (digit < 10) ? '0' + digit : 'A' + (digit - 10);
    }

    // Output the number
    for (int i = 0; i < pos; ++i)
    {
        putc_function(buffer[i]);
    }
}

extern "C"
{
    FILE* stderr = (FILE*)0;
    int fprintf(FILE* __restrict fd, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        RtlGenericPrintf(DbgPrintChar, fmt, args);

        va_end(args);
        return 0;
    }
}