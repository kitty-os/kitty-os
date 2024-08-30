//
// Created by Piotr on 21.07.2024.
//

#include <ccmath/ccmath.hpp>
#include "2d.hpp"

void GfxDrawPixel(
        limine_framebuffer* framebuffer,
        Vector2<size_t> position,
        Vector3<uint8_t> color
)
{
    if (framebuffer == nullptr) return;

    if (position.x >= framebuffer->width || position.y >= framebuffer->height) return;

    size_t offset = (position.y * framebuffer->pitch) + (position.x * (framebuffer->bpp / 8));
    uint8_t* pixelAddress = reinterpret_cast<uint8_t*>(framebuffer->address) + offset;

    uint32_t rMask = (1 << framebuffer->red_mask_size) - 1;
    uint32_t gMask = (1 << framebuffer->green_mask_size) - 1;
    uint32_t bMask = (1 << framebuffer->blue_mask_size) - 1;

    uint32_t rValue = (color.x & rMask) << framebuffer->red_mask_shift;
    uint32_t gValue = (color.y & gMask) << framebuffer->green_mask_shift;
    uint32_t bValue = (color.z & bMask) << framebuffer->blue_mask_shift;

    uint32_t colorValue = rValue | gValue | bValue;

    *reinterpret_cast<uint32_t*>(pixelAddress) = colorValue;
}

void GfxDrawPixelNoOutOfBounds(
        limine_framebuffer* framebuffer,
        Vector2<size_t> position,
        Vector3<uint8_t> color
)
{
    size_t offset = (position.y * framebuffer->pitch) + (position.x * (framebuffer->bpp / 8));
    uint8_t* pixelAddress = reinterpret_cast<uint8_t*>(framebuffer->address) + offset;

    uint32_t rMask = (1 << framebuffer->red_mask_size) - 1;
    uint32_t gMask = (1 << framebuffer->green_mask_size) - 1;
    uint32_t bMask = (1 << framebuffer->blue_mask_size) - 1;

    uint32_t rValue = (color.x & rMask) << framebuffer->red_mask_shift;
    uint32_t gValue = (color.y & gMask) << framebuffer->green_mask_shift;
    uint32_t bValue = (color.z & bMask) << framebuffer->blue_mask_shift;

    uint32_t colorValue = rValue | gValue | bValue;

    *reinterpret_cast<uint32_t*>(pixelAddress) = colorValue;
}

void GfxDrawRectangle(
        limine_framebuffer* framebuffer,
        Vector2<size_t> position,
        Vector3<uint8_t> color,
        Vector2<size_t> dimensions
)
{
    if (framebuffer == nullptr) return;

    // Calculate the maximum dimensions that fit within the framebuffer
    if (position.x >= framebuffer->width || position.y >= framebuffer->height) return;

    if (position.x + dimensions.x > framebuffer->width) {
        dimensions.x = framebuffer->width - position.x;
    }

    if (position.y + dimensions.y > framebuffer->height) {
        dimensions.y = framebuffer->height - position.y;
    }

    size_t x0 = position.x;
    size_t y0 = position.y;
    size_t x1 = position.x + dimensions.x;
    size_t y1 = position.y + dimensions.y;

    for (size_t y = y0; y < y1; ++y) {
        for (size_t x = x0; x < x1; ++x) {
            Vector2<size_t> pixelPos = { x, y };
            GfxDrawPixelNoOutOfBounds(framebuffer, pixelPos, color);
        }
    }
}

void GfxDrawPixelFragShadered(
        limine_framebuffer* framebuffer,
        Vector2<size_t> position,
        Vector3<uint8_t> color,
        std::function<void(const Vector2<size_t>& position, Vector3<uint8_t>&)> basic_shader
)
{
    basic_shader(position, color);
    GfxDrawPixel(framebuffer, position, color);
}

void GfxDrawRectangleFragShadered(
        limine_framebuffer* framebuffer,
        Vector2<size_t> position,
        Vector3<uint8_t> color,
        Vector2<size_t> dimensions,
        std::function<void(const Vector2<size_t>& position, Vector3<uint8_t>&)> basic_shader
)
{
    if (framebuffer == nullptr) return;
    if (position.x >= framebuffer->width || position.y >= framebuffer->height) return;

    size_t x0 = position.x;
    size_t y0 = position.y;
    size_t x1 = ccm::min(position.x + dimensions.x, framebuffer->width);
    size_t y1 = ccm::min(position.y + dimensions.y, framebuffer->height);

    for (size_t y = y0; y < y1; ++y) {
        for (size_t x = x0; x < x1; ++x) {
            Vector2<size_t> pixelPos = { x, y };
            GfxDrawPixelFragShadered(framebuffer, pixelPos, color, basic_shader);
        }
    }
}

void GfxDrawLine(
        limine_framebuffer* framebuffer,
        Vector2<size_t> start,
        Vector2<size_t> end,
        Vector3<uint8_t> color
)
{
    int x1 = start.x;
    int y1 = start.y;
    int x2 = end.x;
    int y2 = end.y;

    int dx = ccm::abs(x2 - x1);
    int dy = -ccm::abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;

    while (true)
    {
        GfxDrawPixel(framebuffer, Vector2<size_t>(x1, y1), color);

        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}