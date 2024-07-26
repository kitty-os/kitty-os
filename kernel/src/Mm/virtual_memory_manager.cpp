//
// Created by Piotr on 24.07.2024.
//

#include <cstdint>
#include <Ke/debug.hpp>
#include "virtual_memory_manager.hpp"
#include "simple_allocator.hpp"
#include "memory_manager.hpp"

struct VmFreelist
{
    uint64_t base_address;
    uint64_t length;
    VmFreelist* next;
};

static VmFreelist* freelist_head = nullptr;

static void MmVmAddEntryToFreelist(uint64_t base_address, uint64_t length)
{
    VmFreelist* entry = (VmFreelist*) MmSaAlloc(sizeof(VmFreelist));
    entry->next = nullptr;
    entry->length = length;
    entry->base_address = base_address;

    if (freelist_head == nullptr)
    {
        freelist_head = entry;

        return;
    }

    entry->next = freelist_head;
    freelist_head = entry;
}

void MmVmInitialize()
{
    auto higher_half_direct_memory_offset = MmGetHigherHalfDirectMemoryOffset();
    auto kernel_address_structure = MmRetrieveKernelAddress();
    auto memory_map = MmRetrieveMemoryMap();
    auto memory_map_entry_count = MmRetrieveMemoryMapEntryCount();

    // Find the highest address.
    uint64_t highest_address = 0;
    for (size_t entry_index = 0; memory_map_entry_count > entry_index; entry_index++)
    {
        auto this_entry = memory_map[entry_index];
        uint64_t top_address = this_entry->length + this_entry->base;

        if (top_address > highest_address)
            highest_address = top_address;
    }

    uint64_t start_address = (higher_half_direct_memory_offset + highest_address + (0x1000 - 1)) & ~(0x1000 -1);
    uint64_t size = kernel_address_structure->virtual_base - start_address;

    MmVmAddEntryToFreelist(start_address, size);
}

uint64_t MmVmAllocateVirtualMemory(size_t alignment, size_t length)
{
    // Ensure alignment is a power of two
    if ((alignment & (alignment - 1)) != 0)
    {
        DbgPrint("Alignment is not a power of two\n");
        return 0;
    }

    VmFreelist* prev = nullptr;
    VmFreelist* current = freelist_head;

    while (current != nullptr)
    {
        // Calculate the aligned base address within the current block
        uint64_t aligned_base_address = (current->base_address + (alignment - 1)) & ~(alignment - 1);

        // Calculate the end address of the requested block
        uint64_t end_address = aligned_base_address + length;

        if (end_address <= current->base_address + current->length)
        {
            // Found a suitable block
            uint64_t original_base = current->base_address;
            uint64_t original_length = current->length;

            // Update the current freelist entry
            if (aligned_base_address == current->base_address)
            {
                // If allocation starts at the base of the current block
                current->base_address += length;
                current->length -= length;
            }
            else
            {
                // Split the current block
                VmFreelist* new_entry = (VmFreelist*) MmSaAlloc(sizeof(VmFreelist));
                new_entry->base_address = end_address;
                new_entry->length = current->length - (end_address - current->base_address);
                new_entry->next = current->next;

                current->length = aligned_base_address - current->base_address;
                current->next = new_entry;
            }

            if (current->length == 0)
            {
                // Remove the current block if it is fully allocated
                if (prev != nullptr)
                {
                    prev->next = current->next;
                }
                else
                {
                    freelist_head = current->next;
                }

                MmSaFree(current, sizeof(VmFreelist));
            }

            return aligned_base_address;
        }

        prev = current;
        current = current->next;
    }

    // No suitable block found
    DbgPrint("No suitable block found for allocation\n");
    return 0;
}
