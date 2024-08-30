//
// Created by Piotr on 19.08.2024.
//

#include <Ke/debug.hpp>
#include <Io/cpu.hpp>

#if UINT32_MAX == UINTPTR_MAX
    #define STACK_CHK_GUARD 0xe2dee396
#else
    #define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif

[[gnu::used]] uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

extern "C"
{

__attribute__((noreturn)) void __stack_chk_fail(void)
{
    DbgPrintf("*** Stack smashing detected ***\n");
    IoHaltProcessor();
    while (true) { asm volatile ("cli; hlt"); }
}

}