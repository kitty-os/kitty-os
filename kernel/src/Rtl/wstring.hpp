#pragma once

#include <wchar.h>
#include <cstddef>

extern "C"
{

    size_t wcslen(const wchar_t *wstr) noexcept;
    int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n) noexcept;
    wchar_t *wmemcpy(wchar_t *dest, const wchar_t *src, size_t n) noexcept;
    wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n) noexcept;

    size_t wcrtomb(char *mbstr, wchar_t wc, mbstate_t *ps) noexcept;
}