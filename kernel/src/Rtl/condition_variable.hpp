#pragma once

#include <Rtl/mutex.hpp>

class ConditionVariable
{
private:
    std::atomic<bool> notified = false;

public:
    ConditionVariable() = default;

    void wait(Mutex& mtx);
    void notify_one();
};