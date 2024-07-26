//
// Created by Piotr on 21.07.2024.
//

#include <Io/cpu.hpp>

namespace std
{
    void __throw_bad_function_call()
    {
        IoHaltProcessor();
    }
}