//
// Created by Piotr on 09.08.2024.
//

#include <Rtl/spinlock.hpp>
#include <Rtl/printf.hpp>
#include "wprintf.hpp"

void RtlGenericWprintf(
        void (*wputc_function)(wchar_t),
        const wchar_t* format,
        va_list args
)
{
    static Spinlock sl;

    sl.lock();

    int temp_int;
    unsigned long temp_ulong;
    unsigned long long temp_ullong;
    char temp_char;
    char* temp_string;
    wchar_t* temp_wstring;
    wchar_t temp_lchr;
    double temp_double;

    for (const wchar_t* p = format; *p != L'\0'; ++p)
    {
        if (*p == L'%')
        {
            ++p;
            switch (*p)
            {
                case L'd' :
                {
                    temp_int = va_arg(args, int);
                    RtlGenericWPutInt(wputc_function, temp_int);
                    break;
                }
                case L'c' :
                {
                    temp_char = static_cast<char>(va_arg(args, int));
                    wputc_function(temp_char);
                    break;
                }
                case L's' :
                {
                    temp_string = va_arg(args, char*);
                    RtlGenericWPutString(wputc_function, temp_string);
                    break;
                }
                case L'x' :
                {
                    temp_int = va_arg(args, int);
                    RtlGenericWPutHexadecimal(wputc_function, temp_int);
                    break;
                }
                case L'f' :
                {
                    temp_double = va_arg(args, double);
                    RtlGenericWPutDouble(wputc_function, temp_double);
                    break;
                }
                case L'l' :
                {
                    ++p;
                    if (*p == L'l' && *(p+1) == L'x')
                    {
                        p++; // Move past 'x'
                        temp_ullong = va_arg(args, unsigned long long);
                        RtlGenericWPutHexadecimal(wputc_function, temp_ullong);
                    }
                    else if (*p == L'x')
                    {
                        temp_ulong = va_arg(args, unsigned long);
                        RtlGenericWPutHexadecimal(wputc_function, temp_ulong);
                    }
                    else if (*p == L's')
                    {
                        temp_wstring = va_arg(args, wchar_t*);
                        RtlGenericWPutWString(wputc_function, temp_wstring);
                        break;
                    }
                    else if (*p == L'c')
                    {
                        temp_lchr = static_cast<wchar_t>(va_arg(args, int));
                        wputc_function( temp_lchr);
                        break;
                    }
                    break;
                }
            }
        }
        else
        {
            wputc_function(*p);
        }
    }

    sl.unlock();
}

void RtlGenericWPutInt(void (*wputc_function)(wchar_t), int integer) {
    // Handle negative numbers
    if (integer < 0)
    {
        wputc_function(L'-');
        integer = -integer;
    }

    // Convert integer to string manually
    wchar_t buffer[12];  // Enough to hold any 32-bit integer (-2147483648 to 2147483647)
    int pos = 0;

    do
    {
        buffer[pos++] = L'0' + (integer % 10);  // Get the last digit
        integer /= 10;  // Remove the last digit
    }
    while (integer > 0);

    // Output the number in reverse order
    while (pos > 0)
    {
        wputc_function(buffer[--pos]);
    }
}

void RtlGenericWPutString(
        void (*wputc_function)(wchar_t),
        const char* string
)
{
    for (const char* p = string; *p != '\0'; ++p)
    {
        wputc_function((char)*p);  // Output each character using the provided function
    }
}

void RtlGenericWPutWString(
        void (*wputc_function)(wchar_t),
        const wchar_t* wstring
)
{
    for (const wchar_t* p = wstring; *p != L'\0'; ++p)
    {
        wputc_function(*p);  // Output each character using the provided function
    }
}

void RtlGenericWPutHexadecimal(void (*wputc_function)(wchar_t), uint64_t hexadecimal) {
    wchar_t buffer[16];  // 16 digits for 64-bit hexadecimal
    int pos = 0;

    // Process each nibble (4 bits) from the highest to the lowest
    for (int i = 60; i >= 0; i -= 4)
    {
        int digit = (hexadecimal >> i) & 0xF;  // Extract 4 bits
        buffer[pos++] = (digit < 10) ? L'0' + digit : L'A' + (digit - 10);
    }

    // Output the number
    for (int i = 0; i < pos; ++i)
    {
        wputc_function(buffer[i]);
    }
}

void RtlGenericWPutDouble(void (*wputc_function)(wchar_t), double value)
{
    // Constants
    const int MAX_INTEGER_DIGITS = 10;  // Maximum digits in integer part
    const int MAX_FRACTION_DIGITS = 6;  // Maximum digits in fractional part

    wchar_t buffer[MAX_INTEGER_DIGITS + MAX_FRACTION_DIGITS + 2];  // "digits + . + digits + \0"
    int pos = 0;

    // Handle negative numbers
    if (value < 0.0)
    {
        wputc_function(L'-');
        value = -value;
    }

    // Extract integer part
    int integer_part = static_cast<int>(value);
    double fractional_part = value - integer_part;

    // Convert integer part
    if (integer_part == 0)
    {
        buffer[pos++] = L'0';
    }
    else
    {
        int int_buffer[MAX_INTEGER_DIGITS];
        int int_pos = 0;

        while (integer_part > 0)
        {
            int_buffer[int_pos++] = integer_part % 10;
            integer_part /= 10;
        }

        while (int_pos > 0)
        {
            buffer[pos++] = L'0' + int_buffer[--int_pos];
        }
    }

    // Convert fractional part
    if (fractional_part > 0.0)
    {
        buffer[pos++] = L'.';

        for (int i = 0; i < MAX_FRACTION_DIGITS; ++i)
        {
            fractional_part *= 10;
            int digit = static_cast<int>(fractional_part);
            buffer[pos++] = L'0' + digit;
            fractional_part -= digit;

            if (fractional_part <= 0.0)
                break;
        }
    }

    buffer[pos] = L'\0';

    // Output the number
    for (int i = 0; i < pos; ++i)
    {
        wputc_function(buffer[i]);
    }
}