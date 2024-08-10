#pragma once

#include <cstdint>

#define IOAPIC_REGSEL       0x00
#define IOAPIC_WINDOW       0x10

#define IOAPIC_ID           0x00
#define IOAPIC_VER          0x01
#define IOAPIC_ARBITRATION  0x02
#define IOAPIC_REDTBL       0x10  // Redirection Table base

void HalIrqIOAPICWrite(uint64_t base, uint64_t offset, uint32_t value);
uint32_t HalIrqIOAPICRead(uint64_t base, uint64_t offset);
void HalIrqIOAPICSetRawRedirectionEntry(uint64_t base, uint64_t entry, uint64_t value);
void HalIrqIOAPICMaskEntry(uint64_t base, uint64_t entry);
void HalIrqIOAPICUnmaskEntry(uint64_t base, uint64_t entry);
void HalIrqIOAPICSetIRQ(uint64_t base,
                        uint64_t irq,
                        uint64_t vector,
                        uint64_t delivery_mode,
                        uint64_t destination_mode,
                        uint64_t level_triggered,
                        uint64_t polarity,
                        uint64_t mask,
                        uint64_t destination
);
void HalIrqInitializeIOAPIC(uint64_t ioapic_address);