#pragma once

#include <cstdint>
#include <cstddef>

void GfxTextInitializeWithTTFFont(void* font_pointer, size_t font_size);
void GfxTextPrintWchar(const wchar_t wchr);
void GfxTextWprintf(const wchar_t* fmt, ...);