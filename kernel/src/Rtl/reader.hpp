#pragma once

#include <cstdint>
#include <cstddef>

struct ReaderContext
{
    char* current_pointer;
    size_t cursor;
};

void RtlReaderCreateContext(ReaderContext* ctx, void* data_pointer);
uint8_t RtlReaderRead8(ReaderContext& ctx);
uint16_t RtlReaderRead16Be(ReaderContext& ctx);
uint32_t RtlReaderRead32Be(ReaderContext& ctx);
uint64_t RtlReaderRead64Be(ReaderContext& ctx);
uint16_t RtlReaderRead16Le(ReaderContext& ctx);
uint32_t RtlReaderRead32Le(ReaderContext& ctx);
uint64_t RtlReaderRead64Le(ReaderContext& ctx);