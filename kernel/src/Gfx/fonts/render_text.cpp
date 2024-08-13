//
// Created by Piotr on 09.08.2024.
//

#include <Gfx/fonts/ttf/ttf.hpp>
#include <Io/framebuffer.hpp>
#include <stb_sprintf.h>
#include <cstdarg>
#include "render_text.hpp"

limine_framebuffer* fb;
TTFContext ttf_context;
Viewport vp;

void GfxTextInitializeWithTTFFont(void* font_pointer, size_t font_size)
{
    IoQueryFramebuffer(&fb, 0);
    if (fb == nullptr) return;

    vp = {fb->width, fb->height, 0, 0};
    GfxCreateTTFontContext(&ttf_context, font_pointer, font_size);
}

void GfxTextWprintf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char buffer[1024];
    stbsp_vsnprintf(buffer, 1024, fmt, args);
    GfxDrawTTFCodepoints(&ttf_context, buffer, fb, vp);

    va_end(args);
}

size_t GfxGetMaxHorizontalCharacters()
{

}

size_t GfxGetMaxVerticalCharacters()
{

}