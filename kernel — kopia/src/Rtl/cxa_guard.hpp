#pragma once

#include <atomic>
#include <cstdint>

struct __cxa_guard {
    std::atomic<uint64_t> value = ATOMIC_VAR_INIT(0);
};

// Functions for acquiring, releasing, and aborting the guard.
extern "C" bool __cxa_guard_acquire(__cxa_guard* guard);
extern "C" void __cxa_guard_release(__cxa_guard* guard);
extern "C" void __cxa_guard_abort(__cxa_guard* guard);
