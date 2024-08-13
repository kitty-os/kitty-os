//
// Created by Piotr on 22.07.2024.
//

#include "string.hpp"

extern "C"
{
size_t strlen(const char *str)
{
    const char *s = str;
    while (*s) ++s;
    return s - str;
}

char *strcpy(char *dest, const char *src)
{
    char *d = dest;
    while ((*d++ = *src++) != '\0');
    return dest;
}

char *strcat(char *dest, const char *src)
{
    char *d = dest;
    while (*d) ++d;
    while ((*d++ = *src++) != '\0');
    return dest;
}

int strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        ++str1;
        ++str2;
    }
    return *(unsigned char *) str1 - *(unsigned char *) str2;
}

void *memset(void *dest, int ch, size_t count)
{
    unsigned char *d = (unsigned char *) dest;
    while (count--)
    {
        *d++ = (unsigned char) ch;
    }
    return dest;
}

void *memcpy(void *dest, const void *src, size_t count)
{
    const unsigned char *s = (const unsigned char *) src;
    unsigned char *d = (unsigned char *) dest;
    while (count--)
    {
        *d++ = *s++;
    }
    return dest;
}

void *memmove(void *dest, const void *src, size_t count)
{
    unsigned char *d = (unsigned char *) dest;
    const unsigned char *s = (const unsigned char *) src;

    if (d < s)
    {
        while (count--)
        {
            *d++ = *s++;
        }
    } else
    {
        d += count;
        s += count;
        while (count--)
        {
            *--d = *--s;
        }
    }
    return dest;
}

int memcmp(const void *ptr1, const void *ptr2, size_t count)
{
    const unsigned char *p1 = (const unsigned char *) ptr1;
    const unsigned char *p2 = (const unsigned char *) ptr2;
    while (count--)
    {
        if (*p1 != *p2)
        {
            return *p1 - *p2;
        }
        ++p1;
        ++p2;
    }
    return 0;
}

char *strncat(char *dest, const char *src, size_t n)
{
    char *d = dest;

    while (*d != '\0')
    {
        d++;
    }

    while (n-- && (*src != '\0'))
    {
        *d++ = *src++;
    }

    *d = '\0';

    return dest;
}
}