//
// Created by Piotr on 04.08.2024.
//

#include <Mm/memory_manager.hpp>
#include <Ke/debug.hpp>
#include <Io/io.hpp>
#include "ioapic.hpp"

void HalIrqIOAPICWrite(uint64_t base, uint64_t offset, uint32_t value)
{
    auto ioapic_base = (uintptr_t) (base + MmGetHigherHalfDirectMemoryOffset());
    *(volatile uint32_t*)(ioapic_base + IOAPIC_REGSEL) = offset;
    *(volatile uint32_t*)(ioapic_base + IOAPIC_WINDOW) = value;
}

uint32_t HalIrqIOAPICRead(uint64_t base, uint64_t offset)
{
    auto ioapic_base = (uintptr_t) (base + MmGetHigherHalfDirectMemoryOffset());
    *(volatile uint32_t*)(ioapic_base + IOAPIC_REGSEL) = offset;
    return *(volatile uint32_t*)(ioapic_base + IOAPIC_WINDOW);
}

void HalIrqIOAPICSetRawRedirectionEntry(uint64_t base, uint64_t entry, uint64_t value)
{
    HalIrqIOAPICWrite(base, IOAPIC_REDTBL + 2 * entry, (uint32_t)value);
    HalIrqIOAPICWrite(base, IOAPIC_REDTBL + 2 * entry + 1, (uint32_t)(value >> 32));
}

void HalIrqIOAPICMaskEntry(uint64_t base, uint64_t entry)
{
    uint64_t current = (uint64_t)HalIrqIOAPICRead(base, IOAPIC_REDTBL + 2 * entry) |
                       ((uint64_t)HalIrqIOAPICRead(base, IOAPIC_REDTBL + 2 * entry + 1) << 32);
    HalIrqIOAPICSetRawRedirectionEntry(base, entry, current | (1ULL << 16));
}

void HalIrqIOAPICUnmaskEntry(uint64_t base, uint64_t entry)
{
    uint64_t current = (uint64_t)HalIrqIOAPICRead(base, IOAPIC_REDTBL + 2 * entry) |
                       ((uint64_t)HalIrqIOAPICRead(base, IOAPIC_REDTBL + 2 * entry + 1) << 32);
    HalIrqIOAPICSetRawRedirectionEntry(base, entry, current & ~(1ULL << 16));
}

void HalIrqIOAPICSetIRQ(uint64_t base,
                        uint64_t irq,
                        uint64_t vector,
                        uint64_t delivery_mode,
                        uint64_t destination_mode,
                        uint64_t level_triggered,
                        uint64_t polarity,
                        uint64_t mask,
                        uint64_t destination
)
{
    uint64_t entry = 0;
    entry |= (vector & 0xFF);                  // Interrupt Vector
    entry |= ((delivery_mode & 0x7) << 8);     // Delivery Mode
    entry |= ((destination_mode & 0x1) << 11); // Destination Mode
    entry |= ((level_triggered & 0x1) << 15);  // Interrupt Input Pin Polarity
    entry |= ((mask & 0x1) << 16);             // Mask
    entry |= ((polarity & 0x1) << 13);         // Trigger Mode
    entry |= ((destination & 0xFF) << 56);     // Destination Field

    HalIrqIOAPICSetRawRedirectionEntry(base, irq, entry);
}

void HalIrqInitializeIOAPIC(uint64_t ioapic_address)
{
    auto v =  HalIrqIOAPICRead(ioapic_address, IOAPIC_VER);

    // what do i do bro
}