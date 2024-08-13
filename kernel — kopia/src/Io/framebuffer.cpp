//
// Created by Piotr on 21.07.2024.
//

#include "framebuffer.hpp"

volatile limine_framebuffer_request framebuffer_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 0,
        .response = nullptr
};

void IoQueryFramebuffer(limine_framebuffer** framebuffer, size_t framebuffer_index)
{
    if (framebuffer == nullptr)
    {
        return;
    }

    if (framebuffer_request.response == nullptr)
    {
        *framebuffer = nullptr;
    }

    if (framebuffer_request.response->framebuffer_count <= framebuffer_index)
    {
        *framebuffer = nullptr;
    }

    *framebuffer = framebuffer_request.response->framebuffers[framebuffer_index];
}