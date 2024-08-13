#pragma once

#include <cstdint>

inline int rdmsr(uint32_t reg, uint64_t *value)
{
    uint32_t low, high;

    __asm__ __volatile__ (
            "rdmsr" : "=a"(low), "=d"(high) : "c"(reg)
            );

    *value = (static_cast<uint64_t>(high) << 32) | low;
    return 0;
}

// Function to write to an MSR register
inline int wrmsr(uint32_t reg, uint64_t value)
{
    uint32_t low = static_cast<uint32_t>(value);
    uint32_t high = static_cast<uint32_t>(value >> 32);

    __asm__ __volatile__ (
            "wrmsr" : : "c"(reg), "a"(low), "d"(high)
            );

    return 0;
}