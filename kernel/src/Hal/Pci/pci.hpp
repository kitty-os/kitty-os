#pragma once

#include <Fw/Acpi/acpi.hpp>
#include <cstdint>

struct PCIDeviceDatabaseEntry
{
    const char* vendor_name;
    const char* device_name;
    const uint16_t vendor_id;
    const uint16_t device_id;
    const uint16_t subvendor_id;
    const uint16_t subdevice_id;
    const char* subsystem_name;
};

struct PCIDeviceCommonHeader
{
    uint16_t vendor_id, device_id;
    uint16_t command, status;
    uint8_t revision_id, prog_if;
    uint8_t subclass, class_code;
    uint8_t cache_line_size, latency_timer;
    uint8_t header_type, bist;
} __attribute__((packed));

struct PCIHeaderType0
{
    PCIDeviceCommonHeader header;
    uint32_t bars[6];
    uint32_t cb_cis_pointer;
    uint16_t subsystem_vendor_id, subsystem_device_id;
    uint32_t expansion_rom_base_address;
    uint8_t capabilities_pointer;
    uint8_t rsvd0;
    uint16_t rsvd1;
    uint32_t rsvd2;
    uint8_t interrupt_line, interrupt_pin;
    uint8_t min_grant, max_latency;
} __attribute__((packed));

enum BARType
{
    BAR_TYPE_MMIO, BAR_TYPE_IO
};

struct InterpretedBAR
{
    uint32_t base_address;
    uint16_t io_address;
    bool is_prefetchable;
    bool is_io_mapped;
    bool is_mmio_mapped;
    uint8_t type;
};

void HalPciInterpretBAR(InterpretedBAR* ibar, uint32_t bar_value);
uintptr_t HalPciAccessPciExpressDevice(uint64_t base_address_virtual, uint64_t bus, uint64_t slot, uint64_t function);
void HalPciScanPciExpressBAAS(MCFGBAAS& baas);
void HalPciInitializePCIExpress();
const PCIDeviceDatabaseEntry* HalPciQueryDeviceName(uint16_t vendor_id, uint16_t device_id, uint16_t subsystem_vendor_id, uint16_t subsystem_device_id);