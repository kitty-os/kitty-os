//
// Created by Piotr on 29.07.2024.
//

#include "reader.hpp"

uint16_t readUInt16(const uint8_t* buf, size_t offset)
{
    return (buf[offset] << 8) | buf[offset + 1];
}

uint32_t readUInt32(const uint8_t* buf, size_t offset)
{
    return (buf[offset] << 24) | (buf[offset + 1] << 16) | (buf[offset + 2] << 8) | buf[offset + 3];
}

int16_t readInt16(const uint8_t* buf, size_t offset)
{
    return (buf[offset] << 8) | buf[offset + 1];
}