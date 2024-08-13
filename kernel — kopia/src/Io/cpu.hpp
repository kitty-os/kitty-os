#pragma once

#include <cstdint>

inline void IoInvalidatePage(void* page)
{
    asm volatile (
            "invlpg (%0)"
            :
            : "r" (page)
            : "memory"
            );
}

extern "C" void IoInitializeExtensions();
uint64_t IoReadCpuCr3();
void IoHaltProcessor();
