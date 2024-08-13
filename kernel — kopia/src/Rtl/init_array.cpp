//
// Created by Piotr on 27.07.2024.
//

#include <cstddef>
#include "init_array.hpp"

extern void (*__init_array[])();
extern void (*__init_array_end[])();

void AbiCallCtors()
{
    for (size_t i = 0; &__init_array[i] != __init_array_end; i++)
    {
        __init_array[i]();
    }
}