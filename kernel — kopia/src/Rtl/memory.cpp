//
// Created by Piotr on 09.08.2024.
//

#include <cstdlib>
#include <Ke/debug.hpp>
#include <Mm/heap.hpp>
#include <Io/cpu.hpp>
#include <cstdint>
#include "memory.hpp"

Heap heap;

void RtlInitializeGlobalHeap()
{
    // 1GB vmem max
    // 1MB pmem init
    heap.HeapInit(1 * 1024 * 1024 * 1024ULL, 1 * 1024 * 1024ULL);
}

void* operator new(std::size_t size)
{
    return heap.AllocateMemory(size);
}

void* operator new[](std::size_t size)
{
    return heap.AllocateMemory(size);
}

void operator delete(void* p) noexcept
{
    heap.DeallocateMemory(p);
}

void operator delete[](void* p) noexcept
{
    heap.DeallocateMemory(p);
}

void* malloc(size_t size)
{
    return heap.AllocateMemory(size);
}

void free(void* ptr)
{
    if (!ptr) return;
    heap.DeallocateMemory(ptr);
}