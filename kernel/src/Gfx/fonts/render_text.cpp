//
// Created by Piotr on 09.08.2024.
//

#include <Gfx/fonts/ttf/ttf.hpp>
#include <Io/framebuffer.hpp>
#include <Rtl/wprintf.hpp>
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

void GfxTextPrintWchar(const wchar_t wchr)
{
    wchar_t buf[2] = {wchr, L'\0'};
    GfxDrawTTFCodepoints(&ttf_context, buf, fb, vp);
}

void GfxTextWprintf(const wchar_t* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    RtlGenericWprintf(GfxTextPrintWchar, fmt, args);
}