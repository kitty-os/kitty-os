//
// Created by Piotr on 24.08.2024.
//

#include "tls.hpp"

void KeTlsSetPointer(void* tls)
{
    __asm__ __volatile__(
            "wrgsbase %0"  // Set the GS base register to the value of tls
            :
            : "r" (tls)  // Input operand
            : "memory"           // Clobber memory
            );
}

size_t KeTlsReadProcessorIndex()
{
    size_t processorIdx;

    size_t offset = offsetof(ThreadLocalStorage, processor_idx);

    __asm__ __volatile__(
            "movq %%gs:(%1), %0"  // Move the value at GS:[offset] to processorIdx
            : "=r" (processorIdx)  // Output operand
            : "r" (offset)         // Input operand
            :                      // No clobbered registers
            );

    return processorIdx;
}

void KeTlsWriteProcessorIndex(size_t processorIndex)
{
    size_t offset = offsetof(ThreadLocalStorage, processor_idx);

    __asm__ __volatile__(
            "movq %0, %%gs:(%1)"  // Move the value of processorIndex to GS:[offset]
            :
            : "r" (processorIndex), "r" (offset)  // Input operands
            : "memory"           // Clobber memory
            );
}