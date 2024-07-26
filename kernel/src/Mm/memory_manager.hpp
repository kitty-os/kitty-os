#pragma once

#include <cstdint>
#include <cstddef>
#include <limine.h>
#include <Ke/status.hpp>

#define PAGE_SIZE 4096

#define PROTECTION_RW           (1 << 0)
#define PROTECTION_KERNEL       (1 << 1)
#define PROTECTION_USER         (1 << 2)
#define PROTECTION_NO_EXECUTE   (1 << 3)
#define PROTECTION_NONE         (0)

#define MAP_NONE        (0)
#define MAP_PRESENT     (1 << 0)
#define MAP_GLOBAL      (1 << 1)

#define MAP_SUCCESS     CREATE_ERROR_CODE(MM_ERROR_BASE, SEVERITY_INFO,     0); // The map was successful.
#define FAILED_TO_MAP   CREATE_ERROR_CODE(MM_ERROR_BASE, SEVERITY_ERROR,    1); // Failed to map the page (other).
#define MISALIGNED_PAGE CREATE_ERROR_CODE(MM_ERROR_BASE, SEVERITY_CRITICAL, 2); // The page isn't aligned to the size of the page.


struct pml4
{
    uint64_t present    : 1; // Is the page present?
    uint64_t rw         : 1; // Can read/write to/from the page?
    uint64_t user       : 1; // Can user access this page?
    uint64_t pwt        : 1; // Page write through
    uint64_t pcd        : 1; // Page cache disable
    uint64_t accesed    : 1; // Is page accessed?
    uint64_t ignored    : 1; // Ignored field.
    uint64_t mbz        : 2; // Must be zero.
    uint64_t ats0       : 3; // Available to software.
    uint64_t pdp_ppn    : 40; // Physical page number to the PDP tables.
    uint64_t ats1       : 11; // Available to the software.
    uint64_t no_execute : 1;  // Disable execution of code on this page.
} __attribute__((packed));
static_assert(sizeof(pml4) == 8, "Structure PML4 isn't 8 bytes big.");

struct pdp
{
    uint64_t present    : 1; // Is the page present?
    uint64_t rw         : 1; // Can read/write to/from the page?
    uint64_t user       : 1; // Can user access this page?
    uint64_t pwt        : 1; // Page write through
    uint64_t pcd        : 1; // Page cache disable
    uint64_t accesed    : 1; // Is page accessed?
    uint64_t ignored    : 1; // Ignored field.
    uint64_t mbz        : 1; // Must be zero.
    uint64_t ignored2   : 1; // Ignored field.
    uint64_t ats0       : 3; // Available to software.
    uint64_t pd_ppn     : 40; // Physical page number to the PD tables.
    uint64_t ats1       : 11; // Available to the software.
    uint64_t no_execute : 1;  // Disable execution of code on this page.
} __attribute__((packed));
static_assert(sizeof(pdp) == 8, "Structure PDP isn't 8 bytes big.");

struct pd
{
    uint64_t present    : 1; // Is the page present?
    uint64_t rw         : 1; // Can read/write to/from the page?
    uint64_t user       : 1; // Can user access this page?
    uint64_t pwt        : 1; // Page write through
    uint64_t pcd        : 1; // Page cache disable
    uint64_t accesed    : 1; // Is page accessed?
    uint64_t ignored    : 1; // Ignored field.
    uint64_t mbz        : 1; // Must be zero.
    uint64_t ignored2   : 1; // Ignored field.
    uint64_t ats0       : 3; // Available to software.
    uint64_t pt_ppn     : 40; // Physical page number to the PT tables.
    uint64_t ats1       : 11; // Available to the software.
    uint64_t no_execute : 1;  // Disable execution of code on this page.
} __attribute__((packed));
static_assert(sizeof(pd) == 8, "Structure PD isn't 8 bytes big.");

struct pt
{
    uint64_t present    : 1; // Is the page present?
    uint64_t rw         : 1; // Can read/write to/from the page?
    uint64_t user       : 1; // Can user access this page?
    uint64_t pwt        : 1; // Page write through
    uint64_t pcd        : 1; // Page cache disable
    uint64_t accesed    : 1; // Is page accessed?
    uint64_t dirty      : 1; // Was the field written to?
    uint64_t pat        : 1; // Page attribute table.
    uint64_t global     : 1; // Is page global (unvalidated)
    uint64_t ats0       : 3; // Available to software.
    uint64_t papn_ppn   : 40; // Physical page number to the physical address.
    uint64_t ats1       : 7;  // Available to the software.
    uint64_t pkeys      : 4;  // Protection keys
    uint64_t no_execute : 1;  // Disable execution of code on this page.
} __attribute__((packed));
static_assert(sizeof(pt) == 8, "Structure PT isn't 8 bytes big.");

struct VirtualAddress
{
    uint64_t padding    : 16;
    uint64_t pml4_index : 9;
    uint64_t pdp_index  : 9;
    uint64_t pd_index   : 9;
    uint64_t pt_index   : 9;
    uint64_t offset     : 12;
};

VirtualAddress MmSplitVirtualAddress(uint64_t address);
uint64_t MmMergeVirtualAddress(VirtualAddress address);
pml4* MmGetKernelPML4();

uintptr_t MmGetHigherHalfDirectMemoryOffset();

void MmInitializeMemoryManager();
void* MmAllocatePage();
void MmDeallocatePage(void* ptr);
bool MmPageExists(pml4* pml4_table, uint64_t virtual_address);
KSTATUS MmMapPage(
        pml4* pml4_table,
        uint64_t virtual_address,
        uint64_t physical_address,
        unsigned int protection_flags,
        unsigned int map_flags
        );
limine_memmap_entry** MmRetrieveMemoryMap();
size_t MmRetrieveMemoryMapEntryCount();
limine_kernel_address_response* MmRetrieveKernelAddress();