//
// Created by Piotr on 21.07.2024.
//

#include "cpu.hpp"

uint64_t IoReadCpuCr3()
{
    uint64_t cr3;
    asm volatile ("mov %%cr3, %0" : "=r"(cr3));
    return cr3;
}

void IoHaltProcessor()
{
    asm volatile ("cli"); // Disable interrupts
    asm volatile ("hlt"); // Halt the processor
}