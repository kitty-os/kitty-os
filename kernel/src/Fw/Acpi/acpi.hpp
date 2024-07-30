#pragma once

#include <cstdint>

struct SDT
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct XSDT
{
    SDT sdt;
    uint64_t tables[];
};

struct RSDT
{
    SDT sdt;
    uint32_t tables[];
};

struct RSDP
{
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;

    uint32_t rsdt_address;
} __attribute__((packed));

struct XSDP
{
    RSDP rsdp;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct MADT
{
    SDT sdt;
    uint32_t lapic_address;
    uint32_t flags;
};

#define MADT_FLAGS_DUAL_8259_PIC 1 << 0

struct MADTEntry
{
    uint8_t entry_type;
    uint8_t entry_length;
};

#define MADT_ENTRY_TYPE_LAPIC 0
#define MADT_ENTRY_TYPE_IOAPIC 1
#define MADT_ENTRY_TYPE_IOAPIC_INTERRUPT_SOURCE_OVERRIDE 2
#define MADT_ENTRY_TYPE_IOAPIC_NONMASKABLE_INTERRUPT_SOURCE 3
#define MADT_ENTRY_TYPE_LAPIC_NON_MASKABLE_INTERRUPTS 4
#define MADT_ENTRY_TYPE_LAPIC_ADDRESS_OVERRIDE 5
#define MADT_ENTRY_TYPE_PROCESSOR_LOCAL_X2APIC 9

struct MADTLAPIC
{
    MADTEntry entry;
    uint8_t acpi_processor_id;
    uint8_t flags;
};

#define MADT_LAPIC_PROCESSOR_ENABLED 1 << 0
#define MADT_LAPIC_ONLINE_CAPABLE 1 << 1

struct MADTIOAPIC
{
    MADTEntry entry;
    uint8_t ioapic_id;
    uint8_t reserved;
    uint32_t ioapic_address;
    uint32_t gsi_base;      // global system interrupt base
};

// ioapic interrupt source override
struct MADTIOAPICISO
{
    MADTEntry entry;
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t gsi; // global system interrupt
    uint16_t flags;
};

// ioapic non maskable interrupt source
struct MADTIOAPICNMIS
{
    MADTEntry entry;
    uint8_t nmi_source;
    uint8_t reserved;
    uint16_t flags;
    uint32_t gsi;
};

// lapic non maskable interrupts
struct MADTLAPICNMI
{
    MADTEntry entry;
    uint8_t acpi_processor_id; // 0xff = all processors
    uint16_t flags;
    uint8_t lint;
};

// lapic address override
struct MADTLAPICAO
{
    MADTEntry entry;
    uint16_t reserved;
    uint64_t lapic_address;
};

// local x2apic
struct MADTLX2APIC
{
    MADTEntry entry;
    uint16_t reserved;
    uint32_t x2apic_id;
    uint32_t flags; // same as lapic flags
    uint32_t acpi_id;
};

void FwInitializeACPI();
bool FwAcpiSupportsXSDT(XSDP* xsdp);
void* FwAcpiQueryTable(char signature[4]);