//
// Created by Piotr on 09.08.2024.
//

#include "wstring.hpp"

extern "C"
{

size_t wcslen(const wchar_t *wstr) noexcept
{
    size_t length = 0;

    while (*wstr != L'\0') {
        ++length;
        ++wstr;
    }

    return length;
}

int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n) noexcept
{
    while (n-- > 0)
    {
        if (*s1 != *s2)
        {
            return (*s1 > *s2) ? 1 : -1;
        }
        s1++;
        s2++;
    }
    return 0;
}

wchar_t *wmemcpy(wchar_t *dest, const wchar_t *src, size_t n) noexcept
{
    wchar_t *d = dest;
    const wchar_t *s = src;
    while (n-- > 0)
    {
        *d++ = *s++;
    }
    return dest;
}

wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n) noexcept
{
    while (n-- > 0)
    {
        if (*s == c)
        {
            return (wchar_t *)s;
        }
        s++;
    }
    return NULL;
}

size_t wcrtomb(char *mbstr, wchar_t wc, mbstate_t *ps) noexcept
{
    // If mbstr is NULL, return the number of bytes needed to represent the character
    if (mbstr == NULL)
    {
        if (wc <= 0x7F) return 1;
        if (wc <= 0x7FF) return 2;
        if (wc <= 0xFFFF) return 3;
        if (wc <= 0x10FFFF) return 4;
        return (size_t)-1;  // Invalid wide character
    }

    // Check for invalid wide characters
    if (wc > 0x10FFFF || (wc >= 0xD800 && wc <= 0xDFFF))
    {
        return (size_t)-1;  // Invalid wide character
    }

    // Encode the wide character as UTF-8
    if (wc <= 0x7F)
    {
        mbstr[0] = (char)wc;
        return 1;
    }
    else if (wc <= 0x7FF)
    {
        mbstr[0] = (char)(0xC0 | (wc >> 6));
        mbstr[1] = (char)(0x80 | (wc & 0x3F));
        return 2;
    }
    else if (wc <= 0xFFFF)
    {
        mbstr[0] = (char)(0xE0 | (wc >> 12));
        mbstr[1] = (char)(0x80 | ((wc >> 6) & 0x3F));
        mbstr[2] = (char)(0x80 | (wc & 0x3F));
        return 3;
    }
    else
    {
        mbstr[0] = (char)(0xF0 | (wc >> 18));
        mbstr[1] = (char)(0x80 | ((wc >> 12) & 0x3F));
        mbstr[2] = (char)(0x80 | ((wc >> 6) & 0x3F));
        mbstr[3] = (char)(0x80 | (wc & 0x3F));
        return 4;
    }
}

wchar_t* wcscpy(wchar_t* dest, const wchar_t* src) noexcept
{
    wchar_t* d = dest;
    const wchar_t* s = src;
    while ((*d++ = *s++) != L'\0')
    {
        // Loop continues until null terminator is copied
    }
    return dest;
}

wchar_t* wmemset(wchar_t* dest, wchar_t wc, std::size_t n) noexcept
{
    wchar_t* d = dest;
    // Set the first n elements of dest to wc
    while (n--)
    {
        *d++ = wc;
    }
    return dest;
}

wchar_t* wmemmove(wchar_t* dest, const wchar_t* src, std::size_t n) noexcept
{
    if (dest == src) {
        return dest;
    }

    if (src < dest && dest < src + n) {
        src += n;
        dest += n;
        while (n--) {
            *(--dest) = *(--src);
        }
    } else {
        while (n--) {
            *dest++ = *src++;
        }
    }

    return dest;
}

}