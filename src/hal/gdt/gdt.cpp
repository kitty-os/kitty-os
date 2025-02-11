#include <hal/gdt/gdt.hpp>

struct gdt_segment_descriptor gdt[] = {
    // NULL
    GDT_ENTRY(0, 0, 0, 0),

    // Kernel Code & data
    GDT_ENTRY(0, 0, GDT_PRESENT | GDT_DPL0 | GDT_EXECUTABLE_BIT | GDT_DESCRIPTOR_TYPE_BIT | GDT_READ_WRITE_BIT,
              GDT_LONG_MODE_FLAG | GDT_GRANULARITY_FLAG),
    GDT_ENTRY(0, 0, GDT_PRESENT | GDT_DPL0 | GDT_DESCRIPTOR_TYPE_BIT | GDT_READ_WRITE_BIT,
              GDT_SIZE_FLAG | GDT_GRANULARITY_FLAG),

    // Drivers Code & data
    GDT_ENTRY(0, 0, GDT_PRESENT | GDT_DPL1 | GDT_EXECUTABLE_BIT | GDT_DESCRIPTOR_TYPE_BIT | GDT_READ_WRITE_BIT,
              GDT_LONG_MODE_FLAG | GDT_GRANULARITY_FLAG),
    GDT_ENTRY(0, 0, GDT_PRESENT | GDT_DPL1 | GDT_DESCRIPTOR_TYPE_BIT | GDT_READ_WRITE_BIT,
              GDT_SIZE_FLAG | GDT_GRANULARITY_FLAG),

    // Program supervisors Code & Data
    GDT_ENTRY(0, 0, GDT_PRESENT | GDT_DPL2 | GDT_EXECUTABLE_BIT | GDT_DESCRIPTOR_TYPE_BIT | GDT_READ_WRITE_BIT,
              GDT_LONG_MODE_FLAG | GDT_GRANULARITY_FLAG),
    GDT_ENTRY(0, 0, GDT_PRESENT | GDT_DPL2 | GDT_DESCRIPTOR_TYPE_BIT | GDT_READ_WRITE_BIT,
              GDT_SIZE_FLAG | GDT_GRANULARITY_FLAG),

    // Kernel code & data
    GDT_ENTRY(0, 0, GDT_PRESENT | GDT_DPL3 | GDT_EXECUTABLE_BIT | GDT_DESCRIPTOR_TYPE_BIT | GDT_READ_WRITE_BIT,
              GDT_LONG_MODE_FLAG | GDT_GRANULARITY_FLAG),
    GDT_ENTRY(0, 0, GDT_PRESENT | GDT_DPL3 | GDT_DESCRIPTOR_TYPE_BIT | GDT_READ_WRITE_BIT,
              GDT_SIZE_FLAG | GDT_GRANULARITY_FLAG),

    // TSS space
    GDT_ENTRY(0, 0, 0, 0),
    GDT_ENTRY(0, 0, 0, 0),
};

void load_gdt(void)
{
    struct gdtr gdtr = {.size = sizeof(gdt) - 1, .gdt_address = reinterpret_cast<uint64_t>(&gdt)};

    flush_gdt(&gdtr);
}