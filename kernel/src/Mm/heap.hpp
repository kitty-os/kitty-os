#pragma once

#include <cstddef>

class Heap
{
public:
    constexpr static size_t alignment = 16;

    explicit Heap(size_t init_vmem, size_t init_pmem);

    void* AllocateMemory(size_t length);
    void DeallocateMemory(void* ptr);

    void PrintFreeList() const;

private:
    struct alignas(16) HeapBlock
    {
        size_t length;
        bool is_allocated;
        HeapBlock *next, *prev;
    };

    void Extend(size_t pages);
    void CoalesceBlocks();

    void* vmem_base;
    size_t vmem_cap;    // Amount of virtual memory allocated (also its the max).
    size_t pmem_size;   // Amount of physical memory allocated to virtual memory.

    HeapBlock* free_list; // Points to the list of free blocks
};
