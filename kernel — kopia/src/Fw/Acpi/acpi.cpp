//
// Created by Piotr on 29.07.2024.
//

#include <Mm/memory_manager.hpp>
#include <Std/string.hpp>
#include <Io/cpu.hpp>
#include <limine.h>
#include "acpi.hpp"

limine_rsdp_request rsdp_request = {
        .id = LIMINE_RSDP_REQUEST,
        .revision = 0,
        .response = nullptr,
};

XSDP* xsdp_table;
bool supports_xsdt;

bool FwAcpiSupportsXSDT(XSDP* xsdp)
{
    return (xsdp->rsdp.revision > 0);
}

void* FwAcpiQueryTable(char signature[4])
{
    if (supports_xsdt)
    {
        XSDT* xsdt_table = (XSDT*) (xsdp_table->xsdt_address + MmGetHigherHalfDirectMemoryOffset());

        size_t entry_count = (xsdt_table->sdt.length - sizeof(xsdt_table->sdt)) / sizeof(uint64_t);

        for (size_t i = 0; entry_count > i; i++)
        {
            SDT* sdt_table = (SDT*) (xsdt_table->tables[i] + MmGetHigherHalfDirectMemoryOffset());

            if (memcmp(sdt_table->signature, signature, 4) == 0)
            {
                return (void*) sdt_table;
            }
        }

        return nullptr;
    }
    else
    {
        RSDT* rsdt_table = (RSDT*) (xsdp_table->rsdp.rsdt_address + MmGetHigherHalfDirectMemoryOffset());

        size_t entry_count = (rsdt_table->sdt.length - sizeof(rsdt_table->sdt)) / sizeof(uint32_t);

        for (size_t i = 0; entry_count > i; i++)
        {
            SDT* sdt_table = (SDT*) ((uint64_t) rsdt_table->tables[i] + MmGetHigherHalfDirectMemoryOffset());

            if (memcmp(sdt_table->signature, signature, 4) == 0)
            {
                return (void*) sdt_table;
            }
        }

        return nullptr;
    }
}

void FwAcpiEnumerateMADTEntries(std::function<void(void* entry)> lambda)
{
    char madt_signature[4] = {'A', 'P', 'I', 'C'};
    MADT* madt = static_cast<MADT*>(FwAcpiQueryTable(madt_signature));

    if (madt == nullptr)
    {
        return;
    }

    uint8_t* entries_start = reinterpret_cast<uint8_t*>(madt) + sizeof(MADT);
    uint8_t* entries_end = reinterpret_cast<uint8_t*>(madt) + madt->sdt.length;

    uint8_t* current_entry = entries_start;
    while (current_entry < entries_end)
    {
        MADTEntry* entry = reinterpret_cast<MADTEntry*>(current_entry);
        lambda(entry);
        current_entry += entry->entry_length;
    }
}

void FwInitializeACPI()
{
    if (rsdp_request.response == nullptr)
    {
        IoHaltProcessor();
    }

    if (rsdp_request.response->address == nullptr)
    {
        IoHaltProcessor();
    }

    xsdp_table = (XSDP*) rsdp_request.response->address;
    supports_xsdt = FwAcpiSupportsXSDT(xsdp_table);
}