#include "cxa_guard.hpp"

extern "C" bool __cxa_guard_acquire(__cxa_guard* guard)
{
    uint64_t expected = 0;
    if (guard->value.load(std::memory_order_acquire) == expected)
    {
        if (guard->value.compare_exchange_strong(expected, 1, std::memory_order_acquire))
        {
            return true;
        } else
        {
            while (guard->value.load(std::memory_order_acquire) == 1)
            {
                // Busy-wait
            }
            return false;
        }
    }
    return false;
}

extern "C" void __cxa_guard_release(__cxa_guard* guard)
{
    guard->value.store(2, std::memory_order_release);
}

extern "C" void __cxa_guard_abort(__cxa_guard* guard)
{
    guard->value.store(0, std::memory_order_release);
}
