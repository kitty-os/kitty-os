//
// Created by Piotr on 06.08.2024.
//

#include <Rtl/string.hpp>
#include "reader.hpp"

void RtlReaderCreateContext(ReaderContext* ctx, void* data_pointer)
{
    ctx->cursor = 0;
    ctx->current_pointer = (char*) data_pointer;
}

// Read 8-bit unsigned integer
uint8_t RtlReaderRead8(ReaderContext& ctx)
{
    uint8_t value = *reinterpret_cast<const uint8_t*>(ctx.current_pointer + ctx.cursor);
    ctx.cursor += sizeof(value);
    return value;
}

// Read 16-bit unsigned integer, big-endian
uint16_t RtlReaderRead16Be(ReaderContext& ctx)
{
    uint16_t value;
    memcpy(&value, ctx.current_pointer + ctx.cursor, sizeof(value));
    ctx.cursor += sizeof(value);
    return __builtin_bswap16(value);
}

// Read 32-bit unsigned integer, big-endian
uint32_t RtlReaderRead32Be(ReaderContext& ctx)
{
    uint32_t value;
    memcpy(&value, ctx.current_pointer + ctx.cursor, sizeof(value));
    ctx.cursor += sizeof(value);
    return __builtin_bswap32(value);
}

// Read 64-bit unsigned integer, big-endian
uint64_t RtlReaderRead64Be(ReaderContext& ctx)
{
    uint64_t value;
    memcpy(&value, ctx.current_pointer + ctx.cursor, sizeof(value));
    ctx.cursor += sizeof(value);
    return __builtin_bswap64(value);
}

// Read 16-bit unsigned integer, little-endian
uint16_t RtlReaderRead16Le(ReaderContext& ctx)
{
    uint16_t value;
    memcpy(&value, ctx.current_pointer + ctx.cursor, sizeof(value));
    ctx.cursor += sizeof(value);
    return value;
}

// Read 32-bit unsigned integer, little-endian
uint32_t RtlReaderRead32Le(ReaderContext& ctx)
{
    uint32_t value;
    memcpy(&value, ctx.current_pointer + ctx.cursor, sizeof(value));
    ctx.cursor += sizeof(value);
    return value;
}

// Read 64-bit unsigned integer, little-endian
uint64_t RtlReaderRead64Le(ReaderContext& ctx)
{
    uint64_t value;
    memcpy(&value, ctx.current_pointer + ctx.cursor, sizeof(value));
    ctx.cursor += sizeof(value);
    return value;
}