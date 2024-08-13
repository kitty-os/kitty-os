//
// Created by Piotr on 08.08.2024.
//

#include "abort.hpp"

#include <Ke/debug.hpp>
#include <Io/cpu.hpp>

extern "C"
{
void abort(void)
{
    IoHaltProcessor();
}

void __assert_fail(const char * assertion, const char * file, unsigned int line, const char * function)
{
    DbgPrintf("[%s %d %s] Assertion failed: %s\n", file, line, function, assertion);
    IoHaltProcessor();
}

}