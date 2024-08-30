//
// Created by Piotr on 21.08.2024.
//

#include "condition_variable.hpp"

void ConditionVariable::wait(Mutex &mtx)
{
    // Release the lock while waiting
    mtx.unlock();
    while (!notified.load(std::memory_order_acquire))
    {
        asm volatile ("xchg %bx, %bx");
    }
    notified.store(false, std::memory_order_release); // Reset the notification state
    mtx.lock(); // Re-acquire the lock after being notified
}

void ConditionVariable::notify_one()
{
    notified.store(true, std::memory_order_release);
}