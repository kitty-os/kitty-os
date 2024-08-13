#pragma once

#include <cstdint>
#include <cstddef>

void GfxTextInitializeWithTTFFont(void* font_pointer, size_t font_size);
void GfxTextWprintf(const char* fmt, ...);