#pragma once

#include <cstdint>

struct GDTRegister
{
    uint16_t limit; // sizeof(gdt_table) - 1
    uint64_t address; // Logical address to GDT table.
} __attribute__((packed));

#define GDT_ACCESS_ACCESSED             1 << 0
#define GDT_ACCESS_RW                   1 << 1
#define GDT_ACCESS_DIRECTION            1 << 2
#define GDT_ACCESS_EXECUTABLE           1 << 3
#define GDT_ACCESS_REGULAR_SEGMENT      1 << 4
#define GDT_ACCESS_DPL(level)           ((level & 0b11) << 5)
#define GDT_ACCESS_PRESENT              1 << 7

#define GDT_FLAGS_LONG          1 << 1
#define GDT_FLAG_SIZE           1 << 2
#define GDT_FLAG_GRANULARITY    1 << 3

struct GDTSegmentDescriptor
{
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t access;
    uint8_t limit1 : 4;
    uint8_t flags : 4;
    uint8_t base2 : 8;
} __attribute__((packed));

#define GDT_ENTRY(access, flags) { \
    0,                             /* limit0 */ \
    0,                             /* base0 */ \
    0,                             /* base1 */ \
    access,                        /* access */ \
    0,                             /* limit1 */ \
    flags,                         /* flags */ \
    0                              /* base2 */ \
}

#define SEGMENT(level) (level * 8)

#define DPL0_CS SEGMENT(1)
#define DPL0_DS SEGMENT(2)

#define DPL1_CS SEGMENT(3)
#define DPL1_DS SEGMENT(4)

#define DPL2_CS SEGMENT(5)
#define DPL2_DS SEGMENT(6)

#define DPL3_CS SEGMENT(7)
#define DPL3_DS SEGMENT(8)

void HalLoadGlobalDescriptorTable();
