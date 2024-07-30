#pragma once

#include <cstdint>
#include <cstddef>

int16_t readInt16(const uint8_t* buf, size_t offset);
uint16_t readUInt16(const uint8_t* buf, size_t offset);
uint32_t readUInt32(const uint8_t* buf, size_t offset);
