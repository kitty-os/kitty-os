#include "spinlock.hpp"

// Constructor: Initializes the atomic flag
Spinlock::Spinlock() : flag() {}

// Method to acquire the spinlock
void Spinlock::lock() {
    while (flag.test_and_set(std::memory_order_acquire)) {
        // Busy-wait (spin) until the lock is available
    }
}

// Method to release the spinlock
void Spinlock::unlock() {
    flag.clear(std::memory_order_release); // Release the lock
}
