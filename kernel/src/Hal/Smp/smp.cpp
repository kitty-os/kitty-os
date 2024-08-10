//
// Created by Piotr on 10.08.2024.
//

#include <Hal/Irq/apic.hpp>
#include <Hal/Int/idt.hpp>
#include <Hal/Mem/gdt.hpp>
#include <Hal/Irq/irq.hpp>
#include <Ke/debug.hpp>
#include <Io/cpu.hpp>
#include <limine.h>
#include "smp.hpp"

limine_smp_request smp_request = {
        .id = LIMINE_SMP_REQUEST,
        .revision = 0,
        .response = nullptr,
        .flags = LIMINE_SMP_X2APIC
};

void HalSmpCoreGotoAddress(limine_smp_info* smp_info)
{
    IoInitializeExtensions();

    HalLoadGlobalDescriptorTable();
    DbgWprintf(L"[CPU%d] Loaded new GDT.\n", smp_info->processor_id);

    HalLoadInterruptDescriptorTable();
    DbgWprintf(L"[CPU%d]Loaded new IDT.\n", smp_info->processor_id);

    HalEnableInterrupts();
    DbgWprintf(L"[CPU%d]Enabled interrupts.\n", smp_info->processor_id);

    HalIrqInitializeLAPIC();
    DbgWprintf(L"[CPU%d]Configured LAPIC.\n", smp_info->processor_id);

    DbgWprintf(L"Started CPU%d.\n", smp_info->processor_id);
    while (1)
    {
        asm volatile ("nop");
    }
}

void HalSmpInitializeLogicalCores()
{
    if (smp_request.response == nullptr)
        return;

    auto response = smp_request.response;

    DbgWprintf(L"CPU count: %d\n", (uint32_t) response->cpu_count);

    for (uint64_t cpu_idx = 0; response->cpu_count > cpu_idx; cpu_idx++)
    {
        auto this_cpu = response->cpus[cpu_idx];

        this_cpu->goto_address = HalSmpCoreGotoAddress;
    }
}