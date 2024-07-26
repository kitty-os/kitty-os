#pragma once
#include <cstdint>

inline void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__ (
            "outb %0, %1"
            :
            : "a" (value), "Nd" (port)
            );
}

inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ __volatile__ (
            "inb %1, %0"
            : "=a" (value)
            : "Nd" (port)
            );
    return value;
}

inline void outw(uint16_t port, uint16_t value) {
    __asm__ __volatile__ (
            "outw %0, %1"
            :
            : "a" (value), "Nd" (port)
            );
}

inline uint16_t inw(uint16_t port) {
    uint16_t value;
    __asm__ __volatile__ (
            "inw %1, %0"
            : "=a" (value)
            : "Nd" (port)
            );
    return value;
}

inline void outl(uint16_t port, uint32_t value) {
    __asm__ __volatile__ (
            "outl %0, %1"
            :
            : "a" (value), "Nd" (port)
            );
}

inline uint32_t inl(uint16_t port) {
    uint32_t value;
    __asm__ __volatile__ (
            "inl %1, %0"
            : "=a" (value)
            : "Nd" (port)
            );
    return value;
}