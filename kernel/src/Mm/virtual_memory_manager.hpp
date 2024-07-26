#pragma once

#include <cstddef>
#include <cstdint>

void MmVmInitialize();
uint64_t MmVmAllocateVirtualMemory(size_t alignment, size_t length);