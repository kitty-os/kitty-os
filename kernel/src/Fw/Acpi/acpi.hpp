#pragma once

#include <cstdint>
#include <functional>

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
} __attribute__((packed));

struct RSDT
{
    SDT sdt;
    uint32_t tables[];
} __attribute__((packed));

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
} __attribute__((packed));

#define MADT_FLAGS_DUAL_8259_PIC 1 << 0

struct MADTEntry
{
    uint8_t entry_type;
    uint8_t entry_length;
} __attribute__((packed));

#define MADT_ENTRY_TYPE_LAPIC 0
#define MADT_ENTRY_TYPE_IOAPIC 1
#define MADT_ENTRY_TYPE_IOAPIC_INTERRUPT_SOURCE_OVERRIDE 2
#define MADT_ENTRY_TYPE_IOAPIC_NON_MASKABLE_INTERRUPT_SOURCE 3
#define MADT_ENTRY_TYPE_LAPIC_NON_MASKABLE_INTERRUPTS 4
#define MADT_ENTRY_TYPE_LAPIC_ADDRESS_OVERRIDE 5
#define MADT_ENTRY_TYPE_PROCESSOR_LOCAL_X2APIC 9

struct MADTLAPIC
{
    MADTEntry entry;
    uint8_t acpi_processor_id;
    uint8_t flags;
} __attribute__((packed));

#define MADT_LAPIC_PROCESSOR_ENABLED 1 << 0
#define MADT_LAPIC_ONLINE_CAPABLE 1 << 1

struct MADTIOAPIC
{
    MADTEntry entry;
    uint8_t ioapic_id;
    uint8_t reserved;
    uint32_t ioapic_address;
    uint32_t gsi_base;      // global system interrupt base
} __attribute__((packed));

// ioapic interrupt source override
struct MADTIOAPICISO
{
    MADTEntry entry;
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t gsi; // global system interrupt
    uint16_t flags;
} __attribute__((packed));

// ioapic non maskable interrupt source
struct MADTIOAPICNMIS
{
    MADTEntry entry;
    uint8_t nmi_source;
    uint8_t reserved;
    uint16_t flags;
    uint32_t gsi;
} __attribute__((packed));

// lapic non maskable interrupts
struct MADTLAPICNMI
{
    MADTEntry entry;
    uint8_t acpi_processor_id; // 0xff = all processors
    uint16_t flags;
    uint8_t lint;
} __attribute__((packed));

// lapic address override
struct MADTLAPICAO
{
    MADTEntry entry;
    uint16_t reserved;
    uint64_t lapic_address;
} __attribute__((packed));

// local x2apic
struct MADTLX2APIC
{
    MADTEntry entry;
    uint16_t reserved;
    uint32_t x2apic_id;
    uint32_t flags; // same as lapic flags
    uint32_t acpi_id;
} __attribute__((packed));

struct MCFGBAAS
{
    uint64_t base_address;
    uint16_t pci_segment;
    uint8_t start_pci_bus, end_pci_bus;
    uint32_t reserved;
};

// MCFG
struct MCFG
{
    SDT sdt;
    uint64_t reserved;
    // Here follows MCFGBAAS structures.
} __attribute__((packed));

struct GENERIC_ADDRESS_STRUCTURE
{
    uint8_t AddressSpace;
    uint8_t BitWidth;
    uint8_t BitOffset;
    uint8_t AccessSize;
    uint64_t Address;
} __attribute__((packed));

struct FADT
{
    struct   SDT h;
    uint32_t FirmwareCtrl;
    uint32_t Dsdt;

    // field used in ACPI 1.0; no longer in use, for compatibility only
    uint8_t  Reserved;

    uint8_t  PreferredPowerManagementProfile;
    uint16_t SCI_Interrupt;
    uint32_t SMI_CommandPort;
    uint8_t  AcpiEnable;
    uint8_t  AcpiDisable;
    uint8_t  S4BIOS_REQ;
    uint8_t  PSTATE_Control;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t  PM1EventLength;
    uint8_t  PM1ControlLength;
    uint8_t  PM2ControlLength;
    uint8_t  PMTimerLength;
    uint8_t  GPE0Length;
    uint8_t  GPE1Length;
    uint8_t  GPE1Base;
    uint8_t  CStateControl;
    uint16_t WorstC2Latency;
    uint16_t WorstC3Latency;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t  DutyOffset;
    uint8_t  DutyWidth;
    uint8_t  DayAlarm;
    uint8_t  MonthAlarm;
    uint8_t  Century;

    // reserved in ACPI 1.0; used since ACPI 2.0+
    uint16_t BootArchitectureFlags;

    uint8_t  Reserved2;
    uint32_t Flags;

    // 12 byte structure; see below for details
    GENERIC_ADDRESS_STRUCTURE ResetReg;

    uint8_t  ResetValue;
    uint8_t  Reserved3[3];

    // 64bit pointers - Available on ACPI 2.0+
    uint64_t                X_FirmwareControl;
    uint64_t                X_Dsdt;

    GENERIC_ADDRESS_STRUCTURE X_PM1aEventBlock;
    GENERIC_ADDRESS_STRUCTURE X_PM1bEventBlock;
    GENERIC_ADDRESS_STRUCTURE X_PM1aControlBlock;
    GENERIC_ADDRESS_STRUCTURE X_PM1bControlBlock;
    GENERIC_ADDRESS_STRUCTURE X_PM2ControlBlock;
    GENERIC_ADDRESS_STRUCTURE X_PMTimerBlock;
    GENERIC_ADDRESS_STRUCTURE X_GPE0Block;
    GENERIC_ADDRESS_STRUCTURE X_GPE1Block;
} __attribute__((packed));

void FwInitializeACPI();
bool FwAcpiSupportsXSDT(XSDP* xsdp);
void FwAcpiEnumerateMADTEntries(std::function<void(void* entry)> lambda);
void* FwAcpiQueryTable(char signature[4]);