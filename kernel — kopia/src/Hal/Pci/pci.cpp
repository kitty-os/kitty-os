//
// Created by Piotr on 09.08.2024.
//

#include <Gfx/fonts/render_text.hpp>
#include <Mm/memory_manager.hpp>
#include <Hal/pci/pci_db.hpp>
#include <Ke/debug.hpp>
#include <vector>
#include "pci.hpp"

MCFG* mcfg_table;
MCFGBAAS* baas_table;
size_t baas_entry_count;
char mcfg_acpi_signature[4] = {'M', 'C', 'F', 'G'};

struct PCIDeviceIdentifier
{
    uint16_t device_id, vendor_id, ss_vendor_id, ss_device_id;
};

std::vector<std::pair<PCIDeviceIdentifier, void*>> pci_express_devices;

void HalPciInitializePCIExpress()
{
    auto table = FwAcpiQueryTable(mcfg_acpi_signature);
    if (table == nullptr)
    {
        GfxTextWprintf(L"Failed to find %s table.\n", mcfg_acpi_signature);
        return;
    }

    mcfg_table = (MCFG*) table;

    GfxTextWprintf(L"Found table %s at address %llx\n", mcfg_acpi_signature, table);

    auto mcfg_size = sizeof(MCFG);
    baas_entry_count = (mcfg_table->sdt.length - mcfg_size) / sizeof(MCFGBAAS);

    baas_table = (MCFGBAAS*)((uintptr_t) (mcfg_table) + sizeof(MCFG));

    for (size_t eidx = 0; baas_entry_count > eidx; eidx++)
    {
        auto baas = baas_table[eidx];

        GfxTextWprintf(L"Entry %d: %llx\n", eidx + 1, baas.base_address);

        HalPciScanPciExpressBAAS(baas);
    }
}

void HalPciScanPciExpressBAAS(MCFGBAAS& baas)
{
    auto beg_bus = baas.start_pci_bus;
    auto end_bus = baas.end_pci_bus;
    auto base_addr = baas.base_address + MmGetHigherHalfDirectMemoryOffset();

    for (size_t current_bus = beg_bus; end_bus > current_bus; current_bus++)
    {
        for (size_t device = 0; 32 > device; device++)
        {
            uint8_t* pcie_device_data = (uint8_t*) HalPciAccessPciExpressDevice(base_addr, current_bus, device, 0);
            PCIDeviceCommonHeader* common_header = (PCIDeviceCommonHeader*) pcie_device_data;

            if (common_header->device_id != 0xffff && common_header->vendor_id != 0xffff)
            {
                if (common_header->header_type == 0)
                {
                    PCIHeaderType0* real_dev = (PCIHeaderType0*) pcie_device_data;

                    //pci_express_devices.push_back(std::make_pair({common_header->vendor_id, common_header->device_id, real_dev->subsystem_vendor_id, real_dev->subsystem_device_id}, (void*)pcie_device_data));

                    auto db_entry = HalPciQueryDeviceName(common_header->vendor_id, common_header->device_id, real_dev->subsystem_vendor_id, real_dev->subsystem_device_id);

                    if (db_entry == nullptr)
                    {
                        GfxTextWprintf(L"No name.\n");
                        continue;
                    }

                    if (db_entry->subsystem_name != nullptr)
                    {
                        GfxTextWprintf(L"[%s] [%s] %s\n",db_entry->vendor_name, db_entry->subsystem_name, db_entry->device_name);
                    }
                    else
                    {
                        GfxTextWprintf(L"[%s] %s\n",db_entry->vendor_name, db_entry->device_name);
                    }
                }
                // multiple function type shi
                else if (common_header->header_type == 128)
                {
                    for (size_t func = 0; 8 > func; func++)
                    {
                        pcie_device_data = (uint8_t*) HalPciAccessPciExpressDevice(base_addr, current_bus, device, func);
                        common_header = (PCIDeviceCommonHeader*) pcie_device_data;
                        common_header->header_type &= ~(0x80);

                        if (common_header->device_id != 0xffff && common_header->vendor_id != 0xffff)
                        {
                            if (common_header->header_type == 0)
                            {
                                PCIHeaderType0* real_dev = (PCIHeaderType0*) pcie_device_data;

                                //pci_express_devices.push_back(std::make_pair({common_header->vendor_id, common_header->device_id, real_dev->subsystem_vendor_id, real_dev->subsystem_device_id}, (void*)pcie_device_data));

                                auto db_entry = HalPciQueryDeviceName(common_header->vendor_id, common_header->device_id, real_dev->subsystem_vendor_id, real_dev->subsystem_device_id);

                                if (db_entry == nullptr)
                                {
                                    GfxTextWprintf(L"No name.\n");
                                    continue;
                                }

                                if (db_entry->subsystem_name != nullptr)
                                {
                                    GfxTextWprintf(L"[%s] [%s] %s\n",db_entry->vendor_name, db_entry->subsystem_name, db_entry->device_name);
                                }
                                else
                                {
                                    GfxTextWprintf(L"[%s] %s\n",db_entry->vendor_name, db_entry->device_name);
                                }
                            }
                            else
                            {
                                GfxTextWprintf(L"Unknown multi-function device header type: %d\n", common_header->header_type);
                            }
                        }

                    }
                }
                else
                {
                    GfxTextWprintf(L"Unknown header type: %d\n", common_header->header_type);
                }
            }
        }
    }
}

const PCIDeviceDatabaseEntry* HalPciQueryDeviceName(uint16_t vendor_id, uint16_t device_id, uint16_t subsystem_vendor_id, uint16_t subsystem_device_id)
{
    for (size_t db_eidx = 0; pci_device_database_size > db_eidx; db_eidx++)
    {
        auto entry = pci_device_database[db_eidx];

        if (entry.vendor_id == vendor_id && entry.device_id == device_id && entry.subvendor_id == subsystem_vendor_id && entry.subdevice_id == subsystem_device_id)
        {
            return &pci_device_database[db_eidx];
        }
    }

    for (size_t db_eidx = 0; pci_device_database_size > db_eidx; db_eidx++)
    {
        auto entry = pci_device_database[db_eidx];

        if (entry.vendor_id == vendor_id && entry.device_id == device_id )
        {
            return &pci_device_database[db_eidx];
        }
    }

    return nullptr;
}

uintptr_t HalPciAccessPciExpressDevice(uint64_t base_address_virtual, uint64_t bus, uint64_t slot, uint64_t function)
{
    return (uintptr_t)(base_address_virtual + (((bus * 256) + (slot * 8) + function) * 4096));
}