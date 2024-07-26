//
// Created by Piotr on 21.07.2024.
//

#include "memory_manager.hpp"
#include <limine/limine.h>
#include <Io/cpu.hpp>
#include <Ke/debug.hpp>
#include <Std/string.hpp>

/// Here I am making the comment that Qwinci is a godsend who told me how to use freelists and how it's beneficial and fast for me to use.

limine_memmap_request memory_map_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};

limine_hhdm_request memory_higher_half_direct_memory_request = {
        .id = LIMINE_HHDM_REQUEST,
        .revision = 0,
        .response = nullptr
};

limine_kernel_address_request kernel_address_request = {
        .id = LIMINE_KERNEL_ADDRESS_REQUEST,
        .revision = 0,
        .response = nullptr
};

uintptr_t hhdm_offset = 0;
limine_memmap_entry** memory_map_entries = nullptr;
size_t memory_map_entry_count = 0;

struct FreeListEntry
{
    size_t length;
    FreeListEntry* next;
};

FreeListEntry* head = nullptr;

void MmInitializeMemoryManager()
{
    if (memory_higher_half_direct_memory_request.response == nullptr ||
        memory_map_request.response == nullptr) {
        IoHaltProcessor();
    }

    memory_map_entries = memory_map_request.response->entries;
    memory_map_entry_count = memory_map_request.response->entry_count;

    hhdm_offset = memory_higher_half_direct_memory_request.response->offset;

    for (size_t entry_index = 0; entry_index < memory_map_entry_count; ++entry_index)
    {
        auto limine_entry = memory_map_entries[entry_index];

        if (limine_entry->type == LIMINE_MEMMAP_USABLE)
        {
            uintptr_t base = limine_entry->base + hhdm_offset;
            size_t length = limine_entry->length;

            if (length > 0)
            {
                FreeListEntry* new_entry = reinterpret_cast<FreeListEntry*>(base);
                new_entry->length = length;
                new_entry->next = nullptr;

                if (head == nullptr) {
                    head = new_entry;
                } else {
                    new_entry->next = head;
                    head = new_entry;
                }
            }
        }
    }
}

pml4* MmGetKernelPML4()
{
    uint64_t cr3_value = IoReadCpuCr3();
    cr3_value &= ~(0xfff); // Mask out the flags to get PPN of PML4.
    cr3_value += hhdm_offset;
    return reinterpret_cast<pml4*>(cr3_value);
}

KSTATUS MmMapPage(
        pml4* pml4_table,
        uint64_t virtual_address,
        uint64_t physical_address,
        unsigned int protection_flags,
        unsigned int map_flags
)
{
    if (pml4_table == nullptr)
    {
        return FAILED_TO_MAP;
    }

    if (((virtual_address % PAGE_SIZE) != 0) || ((physical_address % PAGE_SIZE) != 0))
    {
        return MISALIGNED_PAGE;
    }

    bool is_rw_set = (protection_flags & PROTECTION_RW) != 0;
    bool is_user_set = (protection_flags & PROTECTION_USER) != 0;
    bool is_no_execute_set = (protection_flags & PROTECTION_NO_EXECUTE) != 0;

    bool is_present_set = (map_flags & MAP_PRESENT) != 0;
    bool is_global_set = (map_flags & MAP_GLOBAL) != 0;

    VirtualAddress address = MmSplitVirtualAddress(virtual_address);
    if (pml4_table[address.pml4_index].pdp_ppn == 0)
    {
        auto addr = MmAllocatePage();
        pml4_table[address.pml4_index].pdp_ppn = (uint64_t)addr >> 12;
    }

    pdp* pdp_structure = reinterpret_cast<pdp*>((pml4_table[address.pml4_index].pdp_ppn << 12) + hhdm_offset);
    if (pdp_structure[address.pdp_index].pd_ppn == 0)
    {
        auto addr = MmAllocatePage();
        pdp_structure[address.pdp_index].pd_ppn = (uint64_t)addr >> 12;
    }

    pd* pd_structure = reinterpret_cast<pd*>((pdp_structure[address.pdp_index].pd_ppn << 12) + hhdm_offset);
    if (pd_structure[address.pd_index].pt_ppn == 0)
    {
        auto addr = MmAllocatePage();
        pd_structure[address.pd_index].pt_ppn = (uint64_t)addr >> 12;
    }

    pt* pt_structure = reinterpret_cast<pt*>((pd_structure[address.pd_index].pt_ppn << 12) + hhdm_offset);

    pt_structure[address.pt_index].present = is_present_set;
    pt_structure[address.pt_index].rw = is_rw_set;
    pt_structure[address.pt_index].user = is_user_set;
    pt_structure[address.pt_index].papn_ppn = physical_address >> 12;
    pt_structure[address.pt_index].no_execute = is_no_execute_set;
    pt_structure[address.pt_index].global = is_global_set;

    pd_structure[address.pd_index].present = is_present_set;
    pd_structure[address.pd_index].rw = is_rw_set;
    pd_structure[address.pd_index].user = is_user_set;
    pd_structure[address.pd_index].no_execute = is_no_execute_set;

    pdp_structure[address.pdp_index].present = is_present_set;
    pdp_structure[address.pdp_index].rw = is_rw_set;
    pdp_structure[address.pdp_index].user = is_user_set;
    pdp_structure[address.pdp_index].no_execute = is_no_execute_set;

    pml4_table[address.pml4_index].present = is_present_set;
    pml4_table[address.pml4_index].rw = is_rw_set;
    pml4_table[address.pml4_index].user = is_user_set;
    pml4_table[address.pml4_index].no_execute = is_no_execute_set;

    IoInvalidatePage((void*)virtual_address);

    return MAP_SUCCESS;
}

VirtualAddress MmSplitVirtualAddress(uint64_t address)
{
    VirtualAddress va;
    va.offset = address & 0xFFF;               // Extract the last 12 bits
    va.pt_index = (address >> 12) & 0x1FF;     // Extract the next 9 bits
    va.pd_index = (address >> 21) & 0x1FF;     // Extract the next 9 bits
    va.pdp_index = (address >> 30) & 0x1FF;    // Extract the next 9 bits
    va.pml4_index = (address >> 39) & 0x1FF;   // Extract the next 9 bits
    va.padding = (address >> 48) & 0xFFFF;     // Extract the last 16 bits

    return va;
}

uint64_t MmMergeVirtualAddress(VirtualAddress va)
{
    uint64_t address = 0;
    address |= (static_cast<uint64_t>(va.padding) << 48);
    address |= (static_cast<uint64_t>(va.pml4_index) << 39);
    address |= (static_cast<uint64_t>(va.pdp_index) << 30);
    address |= (static_cast<uint64_t>(va.pd_index) << 21);
    address |= (static_cast<uint64_t>(va.pt_index) << 12);
    address |= va.offset;

    return address;
}

uintptr_t MmGetHigherHalfDirectMemoryOffset()
{
    return hhdm_offset;
}

bool MmPageExists(pml4* pml4_table, uint64_t virtual_address)
{
    if (pml4_table == nullptr)
        return false;

    VirtualAddress va = MmSplitVirtualAddress(virtual_address);

    if (pml4_table[va.pml4_index].present == false || pml4_table[va.pml4_index].pdp_ppn == 0)
        return false;

    uint64_t physical_pointer = pml4_table[va.pml4_index].pdp_ppn << 12;
    pdp* pdp_table = reinterpret_cast<pdp*>(physical_pointer + hhdm_offset);
    if (pdp_table[va.pdp_index].present == false || pdp_table[va.pdp_index].pd_ppn == 0)
        return false;

    physical_pointer = pdp_table[va.pdp_index].pd_ppn << 12;
    pd* pd_table = reinterpret_cast<pd*>(physical_pointer + hhdm_offset);
    if (pd_table[va.pd_index].present == false || pd_table[va.pd_index].pt_ppn == 0)
        return false;

    physical_pointer = pd_table[va.pd_index].pt_ppn << 12;
    pt* pt_table = reinterpret_cast<pt*>(physical_pointer + hhdm_offset);
    if (pt_table[va.pt_index].present == false || pt_table[va.pt_index].papn_ppn == 0)
        return false;

    return true;
}

void* MmAllocatePage()
{
    FreeListEntry* previous = nullptr;
    FreeListEntry* current = head;

    while (current != nullptr)
    {
        if (current->length >= 4096)
        {
            // Allocate the page
            uintptr_t allocated_address = reinterpret_cast<uintptr_t>(current);

            // If the block is larger than a page, reduce its size
            if (current->length > 4096)
            {
                FreeListEntry* new_entry = reinterpret_cast<FreeListEntry*>(allocated_address + 4096);
                new_entry->length = current->length - 4096;
                new_entry->next = current->next;

                if (previous != nullptr)
                {
                    previous->next = new_entry;
                }
                else
                {
                    head = new_entry;
                }
            }
            else
            {
                // Remove the block from the free list
                if (previous != nullptr)
                {
                    previous->next = current->next;
                }
                else
                {
                    head = current->next;
                }
            }

            return reinterpret_cast<void*>(allocated_address - hhdm_offset);
        }

        previous = current;
        current = current->next;
    }

    DbgPrint("No suitable block found.\n");
    return nullptr; // No suitable block found
}

void MmDeallocatePage(void* ptr)
{
    uintptr_t base = reinterpret_cast<uintptr_t>(ptr) + hhdm_offset;
    FreeListEntry* dealloc_entry = reinterpret_cast<FreeListEntry*>(base);
    dealloc_entry->length = 4096; // Assuming pages are always 4096 bytes

    FreeListEntry* current = head;
    FreeListEntry* previous = nullptr;

    while (current != nullptr && reinterpret_cast<uintptr_t>(current) < base)
    {
        previous = current;
        current = current->next;
    }

    if (previous != nullptr)
    {
        previous->next = dealloc_entry;
        dealloc_entry->next = current;

        // Merge with the next block if possible
        if (current != nullptr && (reinterpret_cast<uintptr_t>(dealloc_entry) + dealloc_entry->length) == reinterpret_cast<uintptr_t>(current))
        {
            dealloc_entry->length += current->length;
            dealloc_entry->next = current->next;
        }

        // Merge with the previous block if possible
        if (previous != nullptr && (reinterpret_cast<uintptr_t>(previous) + previous->length) == reinterpret_cast<uintptr_t>(dealloc_entry))
        {
            previous->length += dealloc_entry->length;
            previous->next = dealloc_entry->next;
        }
    }
    else
    {
        dealloc_entry->next = head;
        head = dealloc_entry;

        // Merge with the next block if possible
        if (head->next != nullptr && (reinterpret_cast<uintptr_t>(dealloc_entry) + dealloc_entry->length) == reinterpret_cast<uintptr_t>(head->next))
        {
            dealloc_entry->length += head->next->length;
            dealloc_entry->next = head->next->next;
        }
    }
}

limine_memmap_entry** MmRetrieveMemoryMap()
{
    return memory_map_entries;
}

size_t MmRetrieveMemoryMapEntryCount()
{
    return memory_map_request.response->entry_count;
}

limine_kernel_address_response* MmRetrieveKernelAddress()
{
    return kernel_address_request.response;
}