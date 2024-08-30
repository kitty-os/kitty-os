#pragma once

#include <cstdint>
#include <cstddef>

struct ThreadLocalStorage
{
    size_t processor_idx;
};

void KeTlsSetPointer(void* tls);

size_t KeTlsReadProcessorIndex();
void KeTlsWriteProcessorIndex(size_t processorIndex);