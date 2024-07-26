#pragma once

#include <cstddef>

void* MmSaAlloc(size_t size);
void MmSaFree(void* ptr, size_t size);