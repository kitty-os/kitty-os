//
// Created by Piotr on 24.07.2024.
//

#include <cstddef>
#include "simple_allocator.hpp"
#include "memory_manager.hpp"

struct SAFreelist
{
    size_t size;
    SAFreelist* next;
};

static SAFreelist* head = nullptr;

void* MmSaAlloc(size_t size)
{
    // Align the size to be a multiple of sizeof(SAFreelist*)
    size = (size + sizeof(SAFreelist) - 1) & ~(sizeof(SAFreelist) - 1);

    SAFreelist** current = &head;

    // Traverse the list to find a suitable block
    while (*current)
    {
        if ((*current)->size >= size)
        {
            SAFreelist* block = *current;

            // If the block is exactly the size needed, remove it from the list
            if (block->size == size)
            {
                *current = block->next;
            }
            else
            {
                // Split the block
                SAFreelist* next_block = reinterpret_cast<SAFreelist*>(reinterpret_cast<uintptr_t>(block) + size);
                next_block->size = block->size - size;
                next_block->next = block->next;

                *current = next_block;
                block->size = size;
            }

            return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(block) + sizeof(SAFreelist));
        }

        current = &(*current)->next;
    }

    // No suitable block found, allocate a new page
    auto new_head_ptr = reinterpret_cast<uintptr_t>(MmAllocatePage());
    new_head_ptr += MmGetHigherHalfDirectMemoryOffset();

    SAFreelist* new_block = reinterpret_cast<SAFreelist*>(new_head_ptr);
    new_block->next = head;
    new_block->size = 4096;

    head = new_block;

    return MmSaAlloc(size); // Retry allocation
}

void MmSaFree(void* ptr, size_t size)
{
    if (ptr == nullptr || size == 0)
        return;

    // Align the size to be a multiple of sizeof(SAFreelist*)
    size = (size + sizeof(SAFreelist) - 1) & ~(sizeof(SAFreelist) - 1);

    SAFreelist* block = reinterpret_cast<SAFreelist*>(reinterpret_cast<uintptr_t>(ptr) - sizeof(SAFreelist));
    block->size = size;

    // Insert the block back into the freelist
    SAFreelist** current = &head;

    while (*current && (*current < block))
    {
        current = &(*current)->next;
    }

    block->next = *current;
    *current = block;

    // Merge adjacent free blocks
    if (block->next && reinterpret_cast<uintptr_t>(block) + block->size + sizeof(SAFreelist) == reinterpret_cast<uintptr_t>(block->next))
    {
        block->size += block->next->size + sizeof(SAFreelist);
        block->next = block->next->next;
    }

    if (current != &head && reinterpret_cast<uintptr_t>((*current)->next) == reinterpret_cast<uintptr_t>(block) + block->size + sizeof(SAFreelist))
    {
        (*current)->size += block->next->size + sizeof(SAFreelist);
        (*current)->next = block->next->next;
    }
}