//
// Created by Piotr on 21.08.2024.
//

#include "mutex.hpp"

void Mutex::lock()
{
    while (flag.test_and_set(std::memory_order_acquire))
    {
        asm volatile ("xchg %bx, %bx");
    }
}