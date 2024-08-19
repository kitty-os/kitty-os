
#pragma once
#include <cstddef>

extern "C"
{
    size_t strlen(const char *str);
    char *strcpy(char *dest, const char *src);
    char *strcat(char *dest, const char *src);
    int strcmp(const char *str1, const char *str2);
    char *strncat(char *dest, const char *src, size_t n);
    int strncmp(const char* s1, const char* s2, size_t n);

    // rip isalnum. u already have a definition.

    void *memset(void *dest, int ch, size_t count);
    void *memcpy(void *dest, const void *src, size_t count);
    void *memmove(void *dest, const void *src, size_t count);
    int memcmp(const void *ptr1, const void *ptr2, size_t count);
    void* memchr(const void* ptr, int value, size_t num);
}