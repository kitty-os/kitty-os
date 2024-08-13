#pragma once

#include <stb_truetype.h>
#include <limine/limine.h>
#include <Gfx/2d.hpp>
#include <wchar.h>
#include <cstdint>

struct TTFContext
{
    stbtt_fontinfo font;
    double scale;

    size_t current_x = 0;
    size_t current_y = 0;
    size_t tab_size = 4; // four spaces.

    // ANSI color state
    Vector3<uint8_t> foreground_color = {255, 255, 255}; // Default to white
    Vector3<uint8_t> background_color = {0, 0, 0};       // Default to black
};

struct Viewport {
    size_t width;
    size_t height;
    size_t scroll_x;
    size_t scroll_y;
};

bool GfxCreateTTFontContext(TTFContext* ctx, void* font_buffer, float font_size);
void GfxDrawTTFCodepoint(TTFContext* ctx, wchar_t character, size_t x_position, size_t y_position, limine_framebuffer* framebuffer, const Viewport& viewport);
void GfxDrawTTFCodepoints(TTFContext* ctx, char* characters_nu, limine_framebuffer* framebuffer, const Viewport& viewport);
size_t GfxTTFGetMaxHorizontalCharacters(TTFContext* ctx, const Viewport& viewport);
size_t GfxTTFGetMaxVerticalCharacters(TTFContext* ctx, const Viewport& viewport);