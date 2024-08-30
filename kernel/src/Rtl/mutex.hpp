#pragma once

#include <atomic>

class Mutex
{
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    Mutex() = default;

    void lock();

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};
