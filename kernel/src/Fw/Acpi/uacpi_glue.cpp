//
// Created by Piotr on 19.08.2024.
//

#include <Mm/virtual_memory_manager.hpp>
#include <Mm/memory_manager.hpp>
#include <Ke/debug.hpp>

extern "C"
{
#include <uacpi/kernel_api.h>

uacpi_status uacpi_kernel_raw_memory_read(
        uacpi_phys_addr address, uacpi_u8 byte_width, uacpi_u64 *out_value
)
{
    auto addr = address + MmGetHigherHalfDirectMemoryOffset();

    switch (byte_width)
    {
        case 1:
            *out_value = *(uint8_t*)(addr);
            break;
        case 2:
            *out_value = *(uint16_t*)(addr);
            break;
        case 4:
            *out_value = *(uint32_t*)(addr);
            break;
        case 8:
            *out_value = *(uint64_t*)(addr);
            break;
        default:
            return UACPI_STATUS_INVALID_ARGUMENT;
    }
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_raw_memory_write(
        uacpi_phys_addr address, uacpi_u8 byte_width, uacpi_u64 in_value
)
{
    auto addr = address + MmGetHigherHalfDirectMemoryOffset();

    switch (byte_width)
    {
        case 1:
            *((uint8_t*)addr) = in_value;
            break;
        case 2:
            *((uint16_t*)addr) = in_value;
            break;
        case 4:
            *((uint32_t*)addr) = in_value;
            break;
        case 8:
            *((uint64_t*)addr) = in_value;
            break;
        default:
            return UACPI_STATUS_INVALID_ARGUMENT;
    }
    return UACPI_STATUS_OK;
}

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len)
{
    uintptr_t vaddr = (uintptr_t)MmVmAllocateVirtualMemory(4096, len);

    if (vaddr == 0)
        return NULL;

    size_t num_pages = (len + 4095) / 4096;

    for (size_t i = 0; i < num_pages; i++)
    {
        MmMapPage(
                MmGetKernelPML4(),
                vaddr + (i * 4096),
                addr + (i * 4096),
                PROTECTION_RW | PROTECTION_KERNEL,
                MAP_GLOBAL | MAP_PRESENT
        );
    }

    return (void*)vaddr;
}

void uacpi_kernel_unmap(void *addr, uacpi_size len)
{
    // uhh. we can't deallocate the virtual memory yet.
    // we're kinda limited here.
}

void *uacpi_kernel_alloc(uacpi_size size)
{
    return (void*)(new uint8_t[size]);
}

void uacpi_kernel_free(void *mem)
{
    delete[] (uint8_t*) mem;
}

void *uacpi_kernel_calloc(uacpi_size count, uacpi_size size)
{
    return (void*)(new uint8_t[count * size]);
}

void uacpi_kernel_stall(uacpi_u8 usec)
{
    for (size_t i = 0; (usec * 100) > i; i++) { asm volatile ("xchg %bx, %bx"); }
}

#ifndef UACPI_FORMATTED_LOGGING
    void uacpi_kernel_log(uacpi_log_level lvl, const uacpi_char* msg)
    {
        DbgPrintf("%s", msg);
    }
#else
    UACPI_PRINTF_DECL(2, 3)
    void uacpi_kernel_log(uacpi_log_level lvl, const uacpi_char* msg, ...);
    void uacpi_kernel_vlog(uacpi_log_level lvl, const uacpi_char* msg, uacpi_va_list);
#endif



}

