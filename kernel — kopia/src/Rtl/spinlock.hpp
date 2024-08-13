#pragma once

#include <atomic>

class Spinlock {
public:
    Spinlock();            // Constructor to initialize the spinlock
    void lock();           // Method to acquire the lock
    void unlock();         // Method to release the lock

private:
    std::atomic_flag flag; // Atomic flag for the spinlock
};
