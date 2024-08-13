#include "heap.hpp"
#include <Mm/virtual_memory_manager.hpp>
#include <Mm/memory_manager.hpp>
#include <Ke/debug.hpp>
#include <Io/cpu.hpp>
#include <cstdint>
#include <cstring> // For std::memset

void Heap::HeapInit(size_t init_vmem, size_t init_pmem)
{
    free_list = nullptr;

    // Insanity checks
    if (init_pmem > init_vmem || (init_pmem % 4096) != 0)
    {
        DbgPrint("Invalid memory parameters.\n");
        IoHaltProcessor();
    }

    vmem_base = (void*) MmVmAllocateVirtualMemory(4096, init_vmem);
    if (!vmem_base)
    {
        DbgPrint("Failed to allocate virtual memory.\n");
        IoHaltProcessor();
    }

    for (size_t pg_off = 0; pg_off < init_pmem; pg_off += 4096)
    {
        auto pg_paddr = MmAllocatePage();
        if (pg_paddr == 0) // Check if page allocation was successful
        {
            DbgPrint("Failed to allocate physical page.\n");
            IoHaltProcessor();
        }

        auto status = MmMapPage(
                MmGetKernelPML4(),
                reinterpret_cast<uint64_t>(vmem_base) + pg_off,
                (uint64_t) pg_paddr,
                PROTECTION_KERNEL | PROTECTION_RW,
                MAP_GLOBAL | MAP_PRESENT
        );

        if (status != MAP_SUCCESS)
        {
            DbgPrint("Failed to map some heap pages.\n");
            IoHaltProcessor();
        }
    }

    pmem_size = init_pmem;
    vmem_cap = init_vmem;

    // Initialize free list with the entire heap space
    free_list = reinterpret_cast<HeapBlock*>(vmem_base);
    free_list->length = init_pmem - sizeof(HeapBlock);
    free_list->is_allocated = false;
    free_list->next = nullptr;
    free_list->prev = nullptr;
}

void* Heap::AllocateMemory(size_t length)
{
    // Align the requested length to the block alignment size
    length = (length + alignment - 1) & ~(alignment - 1);

    // Traverse the free list to find a suitable block
    HeapBlock* current = free_list;
    while (current)
    {
        if (!current->is_allocated && current->length >= length)
        {
            // If the block is large enough, split it
            if (current->length > length + sizeof(HeapBlock))
            {
                HeapBlock* new_block = reinterpret_cast<HeapBlock*>(
                        reinterpret_cast<uint8_t*>(current) + sizeof(HeapBlock) + length
                );
                new_block->length = current->length - length - sizeof(HeapBlock);
                new_block->is_allocated = false;
                new_block->next = current->next;
                new_block->prev = current;

                if (current->next)
                {
                    current->next->prev = new_block;
                }
                current->next = new_block;

                current->length = length;
            }

            current->is_allocated = true;
            return reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(current) + sizeof(HeapBlock));
        }
        current = current->next;
    }

    // If no suitable block is found, extend the heap
    Extend(1);
    return AllocateMemory(length);
}

void Heap::DeallocateMemory(void* ptr)
{
    if (!ptr) return;

    // Get the block associated with the pointer
    HeapBlock* block = reinterpret_cast<HeapBlock*>(reinterpret_cast<uint8_t*>(ptr) - sizeof(HeapBlock));
    block->is_allocated = false;

    // Coalesce free blocks if possible
    CoalesceBlocks();
}

void Heap::Extend(size_t pages)
{
    size_t new_pmem_size = pmem_size + pages * 4096;

    if (new_pmem_size > vmem_cap)
    {
        DbgWprintf(L"Heap out of virtual memory %d %llx > %llx\n", (uint32_t)pages, (new_pmem_size), (vmem_cap));
        IoHaltProcessor();
    }

    for (size_t pg_off = pmem_size; pg_off < new_pmem_size; pg_off += 4096)
    {
        auto pg_paddr = MmAllocatePage();
        if (pg_paddr == 0)
        {
            DbgPrint("Failed to allocate physical page.\n");
            IoHaltProcessor();
        }

        auto status = MmMapPage(
                MmGetKernelPML4(),
                reinterpret_cast<uint64_t>(vmem_base) + pg_off,
                (uint64_t) pg_paddr,
                PROTECTION_KERNEL | PROTECTION_RW,
                MAP_GLOBAL | MAP_PRESENT
        );

        if (status != MAP_SUCCESS)
        {
            DbgPrint("Failed to map heap pages during extension.\n");
            IoHaltProcessor();
        }
    }

    pmem_size = new_pmem_size;

    // Add the newly allocated memory to the free list
    HeapBlock* new_block = reinterpret_cast<HeapBlock*>(
            reinterpret_cast<uint8_t*>(vmem_base) + pmem_size - pages * 4096
    );

    new_block->length = pages * 4096 - sizeof(HeapBlock);
    new_block->is_allocated = false;
    new_block->next = nullptr;

    HeapBlock* current = free_list;
    while (current->next)
    {
        current = current->next;
    }

    current->next = new_block;
    new_block->prev = current;

    // Coalesce blocks after extension
    CoalesceBlocks();
}

void Heap::CoalesceBlocks()
{
    HeapBlock* current = free_list;
    while (current && current->next)
    {
        if (!current->is_allocated && !current->next->is_allocated)
        {
            current->length += sizeof(HeapBlock) + current->next->length;
            current->next = current->next->next;

            if (current->next)
            {
                current->next->prev = current;
            }
        }
        else
        {
            current = current->next;
        }
    }
}

void Heap::PrintFreeList() const
{
    HeapBlock* current = free_list;
    while (current)
    {
        DbgPrintf("Block at %llx, Size: %d, Allocated: %s\n",
                 current, current->length,
                 current->is_allocated ? "Yes" : "No");
        current = current->next;
    }
}
