//
// Created by Piotr on 10.08.2024.
//

#include <Ke/debug.hpp>
#include <Io/cpu.hpp>

extern "C"
{
    void* __dso_handle;

    int __cxa_atexit(void (*func)(void*), void* arg, void* dso_handle)
    {
        return 0; // Do nothing
    }

    void __cxa_pure_virtual()
    {
        DbgPrintf("Pure virtual function called.");
        IoHaltProcessor();
    }
}