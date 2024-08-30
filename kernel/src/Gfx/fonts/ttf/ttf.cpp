#include <Rtl/string.hpp>
#include <Gfx/2d.hpp>
#include <codecvt>
#include <string>
#include <locale>
#include "ttf.hpp"

size_t GfxTTFGetMaxHorizontalCharacters(TTFContext* ctx, const Viewport& viewport)
{
    // Get font metrics
    int ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&ctx->font, &ascent, &descent, &line_gap);

    // Calculate character width
    int advance_width, left_side_bearing;
    stbtt_GetCodepointHMetrics(&ctx->font, L'W', &advance_width, &left_side_bearing); // 'W' is typically a wide character

    // Character width in pixels
    int char_width = advance_width * ctx->scale;

    // Calculate the maximum number of characters that fit horizontally
    size_t max_horizontal_chars = viewport.width / char_width;

    return max_horizontal_chars;
}

size_t GfxTTFGetMaxVerticalCharacters(TTFContext* ctx, const Viewport& viewport)
{
    // Get font metrics
    int ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&ctx->font, &ascent, &descent, &line_gap);

    // Calculate line height
    int line_height = ctx->scale * (ascent - descent + line_gap);

    // Calculate the maximum number of lines that fit vertically
    size_t max_vertical_lines = viewport.height / line_height;

    return max_vertical_lines;
}


bool ParseAnsiColor(const std::wstring& text, size_t& index, TTFContext* ctx)
{
    if (text[index] != L'\033' || text.substr(index, 2) != L"\033[")
    {
        return false;
    }

    // Find the end of the color code
    size_t end_index = text.find(L'm', index);
    if (end_index == std::wstring::npos) {
        return false;
    }

    std::wstring color_code = text.substr(index + 2, end_index - index - 2);
    index = end_index + 1; // Move past the color code

    // Parse the color codes
    size_t start = 0;
    size_t pos = color_code.find(L';');

    while (pos != std::wstring::npos) {
        std::wstring code = color_code.substr(start, pos - start);
        if (code == L"0") {
            // Reset to default colors
            ctx->foreground_color = {255, 255, 255};
            ctx->background_color = {0, 0, 0};
        } else if (code == L"1") {
            // Bold (not directly related to color, but could be used for styling)
        } else if (code == L"3") {
            // Italics (not directly related to color, but could be used for styling)
        } else if (code == L"4") {
            // Underline (not directly related to color, but could be used for styling)
        } else if (code == L"30") {
            ctx->foreground_color = {0, 0, 0}; // Black
        } else if (code == L"31") {
            ctx->foreground_color = {255, 0, 0}; // Red
        } else if (code == L"32") {
            ctx->foreground_color = {0, 255, 0}; // Green
        } else if (code == L"33") {
            ctx->foreground_color = {255, 255, 0}; // Yellow
        } else if (code == L"34") {
            ctx->foreground_color = {0, 0, 255}; // Blue
        } else if (code == L"35") {
            ctx->foreground_color = {255, 0, 255}; // Magenta
        } else if (code == L"36") {
            ctx->foreground_color = {0, 255, 255}; // Cyan
        } else if (code == L"37") {
            ctx->foreground_color = {192, 192, 192}; // Light Gray
        } else if (code == L"40") {
            ctx->background_color = {0, 0, 0}; // Black
        } else if (code == L"41") {
            ctx->background_color = {255, 0, 0}; // Red
        } else if (code == L"42") {
            ctx->background_color = {0, 255, 0}; // Green
        } else if (code == L"43") {
            ctx->background_color = {255, 255, 0}; // Yellow
        } else if (code == L"44") {
            ctx->background_color = {0, 0, 255}; // Blue
        } else if (code == L"45") {
            ctx->background_color = {255, 0, 255}; // Magenta
        } else if (code == L"46") {
            ctx->background_color = {0, 255, 255}; // Cyan
        } else if (code == L"47") {
            ctx->background_color = {192, 192, 192}; // Light Gray
        } else if (code == L"90") {
            ctx->foreground_color = {128, 128, 128}; // Dark Gray
        } else if (code == L"91") {
            ctx->foreground_color = {255, 0, 0}; // Bright Red
        } else if (code == L"92") {
            ctx->foreground_color = {0, 255, 0}; // Bright Green
        } else if (code == L"93") {
            ctx->foreground_color = {255, 255, 0}; // Bright Yellow
        } else if (code == L"94") {
            ctx->foreground_color = {0, 0, 255}; // Bright Blue
        } else if (code == L"95") {
            ctx->foreground_color = {255, 0, 255}; // Bright Magenta
        } else if (code == L"96") {
            ctx->foreground_color = {0, 255, 255}; // Bright Cyan
        } else if (code == L"97") {
            ctx->foreground_color = {255, 255, 255}; // Bright White
        } else if (code == L"100") {
            ctx->background_color = {128, 128, 128}; // Dark Gray
        } else if (code == L"101") {
            ctx->background_color = {255, 0, 0}; // Bright Red
        } else if (code == L"102") {
            ctx->background_color = {0, 255, 0}; // Bright Green
        } else if (code == L"103") {
            ctx->background_color = {255, 255, 0}; // Bright Yellow
        } else if (code == L"104") {
            ctx->background_color = {0, 0, 255}; // Bright Blue
        } else if (code == L"105") {
            ctx->background_color = {255, 0, 255}; // Bright Magenta
        } else if (code == L"106") {
            ctx->background_color = {0, 255, 255}; // Bright Cyan
        } else if (code == L"107") {
            ctx->background_color = {255, 255, 255}; // Bright White
        } else {
            // Handle unsupported codes if necessary
        }

        start = pos + 1;
        pos = color_code.find(L';', start);
    }

    // Handle the last code after the final ';'
    std::wstring code = color_code.substr(start);
    if (code == L"0") {
        ctx->foreground_color = {255, 255, 255};
        ctx->background_color = {0, 0, 0};
    } else if (code == L"1") {
        // Bold (not directly related to color, but could be used for styling)
    } else if (code == L"3") {
        // Italics (not directly related to color, but could be used for styling)
    } else if (code == L"4") {
        // Underline (not directly related to color, but could be used for styling)
    } else if (code == L"30") {
        ctx->foreground_color = {0, 0, 0}; // Black
    } else if (code == L"31") {
        ctx->foreground_color = {255, 0, 0}; // Red
    } else if (code == L"32") {
        ctx->foreground_color = {0, 255, 0}; // Green
    } else if (code == L"33") {
        ctx->foreground_color = {255, 255, 0}; // Yellow
    } else if (code == L"34") {
        ctx->foreground_color = {0, 0, 255}; // Blue
    } else if (code == L"35") {
        ctx->foreground_color = {255, 0, 255}; // Magenta
    } else if (code == L"36") {
        ctx->foreground_color = {0, 255, 255}; // Cyan
    } else if (code == L"37") {
        ctx->foreground_color = {192, 192, 192}; // Light Gray
    } else if (code == L"40") {
        ctx->background_color = {0, 0, 0}; // Black
    } else if (code == L"41") {
        ctx->background_color = {255, 0, 0}; // Red
    } else if (code == L"42") {
        ctx->background_color = {0, 255, 0}; // Green
    } else if (code == L"43") {
        ctx->background_color = {255, 255, 0}; // Yellow
    } else if (code == L"44") {
        ctx->background_color = {0, 0, 255}; // Blue
    } else if (code == L"45") {
        ctx->background_color = {255, 0, 255}; // Magenta
    } else if (code == L"46") {
        ctx->background_color = {0, 255, 255}; // Cyan
    } else if (code == L"47") {
        ctx->background_color = {192, 192, 192}; // Light Gray
    } else if (code == L"90") {
        ctx->foreground_color = {128, 128, 128}; // Dark Gray
    } else if (code == L"91") {
        ctx->foreground_color = {255, 0, 0}; // Bright Red
    } else if (code == L"92") {
        ctx->foreground_color = {0, 255, 0}; // Bright Green
    } else if (code == L"93") {
        ctx->foreground_color = {255, 255, 0}; // Bright Yellow
    } else if (code == L"94") {
        ctx->foreground_color = {0, 0, 255}; // Bright Blue
    } else if (code == L"95") {
        ctx->foreground_color = {255, 0, 255}; // Bright Magenta
    } else if (code == L"96") {
        ctx->foreground_color = {0, 255, 255}; // Bright Cyan
    } else if (code == L"97") {
        ctx->foreground_color = {255, 255, 255}; // Bright White
    } else if (code == L"100") {
        ctx->background_color = {128, 128, 128}; // Dark Gray
    } else if (code == L"101") {
        ctx->background_color = {255, 0, 0}; // Bright Red
    } else if (code == L"102") {
        ctx->background_color = {0, 255, 0}; // Bright Green
    } else if (code == L"103") {
        ctx->background_color = {255, 255, 0}; // Bright Yellow
    } else if (code == L"104") {
        ctx->background_color = {0, 0, 255}; // Bright Blue
    } else if (code == L"105") {
        ctx->background_color = {255, 0, 255}; // Bright Magenta
    } else if (code == L"106") {
        ctx->background_color = {0, 255, 255}; // Bright Cyan
    } else if (code == L"107") {
        ctx->background_color = {255, 255, 255}; // Bright White
    } else {
        // Handle unsupported codes if necessary
    }

    return true;
}

// Initialize the font context with the font data
bool GfxCreateTTFontContext(TTFContext* ctx, void* font_buffer, float font_size)
{
    if (!stbtt_InitFont(&ctx->font, (unsigned char*)font_buffer, stbtt_GetFontOffsetForIndex((unsigned char*)font_buffer, 0)))
    {
        return false;
    }

    // Set the scale based on the desired font size
    ctx->scale = stbtt_ScaleForPixelHeight(&ctx->font, font_size);

    return true;
}

void GfxDrawTTFCodepoint(TTFContext* ctx, wchar_t character, size_t x_position, size_t y_position, limine_framebuffer* framebuffer, const Viewport& viewport)
{
    int width, height, x_offset, y_offset;

    // Get the bitmap for the specified character
    unsigned char* bitmap = stbtt_GetCodepointBitmap(&ctx->font, 0, ctx->scale, character, &width, &height, &x_offset, &y_offset);

    if (!bitmap) {
        return; // If bitmap generation fails, exit the function
    }

    // Retrieve font vertical metrics
    int ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&ctx->font, &ascent, &descent, &line_gap);

    // Calculate baseline: y_position is the baseline of the entire string
    int baseline_y = y_position + (int)(ctx->scale * ascent);

    // Calculate the exact y-position to start drawing the bitmap using bounding box information
    int x0, y0, x1, y1;
    stbtt_GetCodepointBitmapBox(&ctx->font, character, ctx->scale, ctx->scale, &x0, &y0, &x1, &y1);

    int y_start = baseline_y + y0; // y0 is usually negative, so this moves the bitmap up to align properly

    // Draw each pixel from the character bitmap onto the framebuffer, considering scrolling
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            int alpha = bitmap[j * width + i];

            if (alpha > 0) {
                // Calculate the exact position on the framebuffer with scrolling applied
                size_t draw_x = x_position + i + x_offset - viewport.scroll_x;
                size_t draw_y = y_start + j - viewport.scroll_y;

                // Check if the pixel is within the viewport bounds
                if (draw_x >= 0 && draw_x < viewport.width && draw_y >= 0 && draw_y < viewport.height) {
                    Vector2<size_t> position = { draw_x, draw_y };

                    // Calculate the final color using the alpha value
                    Vector3<uint8_t> foreground_color = ctx->foreground_color;
                    Vector3<uint8_t> background_color = ctx->background_color;

                    // Simple alpha blending: alpha is the weight for the foreground color
                    uint8_t r = (foreground_color.x * alpha + background_color.x * (255 - alpha)) / 255;
                    uint8_t g = (foreground_color.y * alpha + background_color.y * (255 - alpha)) / 255;
                    uint8_t b = (foreground_color.z * alpha + background_color.z * (255 - alpha)) / 255;

                    Vector3<uint8_t> color = { r, g, b };

                    // Draw the pixel using the blended color
                    GfxDrawPixel(framebuffer, position, color);
                }
            }
        }
    }

    // Free the bitmap memory
    stbtt_FreeBitmap(bitmap, NULL);
}

void GfxClearScreen(limine_framebuffer* framebuffer, const Viewport& viewport, const Vector3<uint8_t>& color) {
    for (size_t y = 0; y < viewport.height; ++y) {
        for (size_t x = 0; x < viewport.width; ++x) {
            GfxDrawPixel(framebuffer, {x, y}, color);
        }
    }
}

wchar_t* ConvertToWchar(const char* utf8_str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide_string = converter.from_bytes(utf8_str);

    wchar_t* wchar_str = new wchar_t[wide_string.size() + 1];
    std::wcscpy(wchar_str, wide_string.c_str());

    return wchar_str;
}

void GfxDrawTTFCodepoints(TTFContext* ctx, char* characters_nu, limine_framebuffer* framebuffer, const Viewport& viewport)
{
    auto characters = ConvertToWchar(characters_nu);

    size_t len = wcslen(characters);
    size_t i = 0;

    int ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&ctx->font, &ascent, &descent, &line_gap);

    int line_height = ctx->scale * (ascent - descent + line_gap);

    while (i < len)
    {
        wchar_t character = characters[i];

        // Handle color codes
        if (ParseAnsiColor(characters, i, ctx)) {
            continue;
        }

        // Get the width of the current character
        int advance_width, left_side_bearing;
        stbtt_GetCodepointHMetrics(&ctx->font, character, &advance_width, &left_side_bearing);

        // Calculate the width of the character in pixels
        int char_width = advance_width * ctx->scale;

        // Handle line wrapping
        if (ctx->current_x + char_width > viewport.width) {
            // Move to the next line
            ctx->current_y += line_height;
            ctx->current_x = 0;
        }

        // Handle tab and newline characters separately
        if (character == L'\t') {
            // Handle tab characters
            int tab_width = ctx->tab_size * (ctx->scale * advance_width);
            ctx->current_x += tab_width;
        } else if (character == L'\n') {
            // Move to the next line
            ctx->current_y += line_height;
            ctx->current_x = 0;
        } else {
            // Draw the character
            GfxDrawTTFCodepoint(ctx, character, ctx->current_x, ctx->current_y, framebuffer, viewport);

            // Update x position for the next character
            int kern_advance = stbtt_GetCodepointKernAdvance(&ctx->font, character, characters[i + 1]);
            ctx->current_x += (advance_width + kern_advance) * ctx->scale;
        }

        // Check for y-axis overflow and scroll if necessary
        if (ctx->current_y + ascent * ctx->scale >= static_cast<int>(viewport.height)) {
            // Calculate how much to scroll
            size_t scroll_amount = line_height;

            // Move the framebuffer content up by the scroll amount using memmove
            size_t bytes_per_pixel = framebuffer->bpp / 8;
            size_t line_size = framebuffer->pitch;
            uint8_t* framebuffer_start = static_cast<uint8_t*>(framebuffer->address);

            memmove(
                    framebuffer_start,                            // Destination address
                    framebuffer_start + scroll_amount * line_size, // Source address
                    (viewport.height - scroll_amount) * line_size  // Number of bytes to move
            );

            // Clear the bottom part of the viewport that was scrolled into view
            Vector2<size_t> clear_position = {0, static_cast<size_t>(viewport.height - scroll_amount)};
            Vector2<size_t> clear_dimensions = {viewport.width, scroll_amount};
            GfxDrawRectangle(framebuffer, clear_position, ctx->background_color, clear_dimensions);

            // Adjust the current_y position to remain within the viewport
            ctx->current_y = viewport.height - line_height;
        }

        ++i;
    }

    delete[] characters;
}
