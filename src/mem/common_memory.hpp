#pragma once

#include <limine.h>
#include <stdint.h>

uintptr_t get_higher_half_offset();
uint64_t get_kernel_base_address();
uint64_t get_highest_memory_map_address();
limine_memmap_response *get_memory_map();
