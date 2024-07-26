#pragma once

#include <cstdint>

#define KSTATUS uint64_t

/// Note: All modules of the kernel have assigned their own error base so they use can use KSTATUS as intended and without having one file containing all the errors.

#define MM_ERROR_BASE  0x0000000000000000
#define GFX_ERROR_BASE 0x0010000000000000
#define IO_ERROR_BASE  0x0020000000000000
#define KE_ERROR_BASE  0x0030000000000000

#define SEVERITY_INFO     0x1000000000000000
#define SEVERITY_WARN     0x2000000000000000
#define SEVERITY_ERROR    0x3000000000000000
#define SEVERITY_CRITICAL 0x4000000000000000
#define SEVERITY_DEBUG    0x5000000000000000

#define CREATE_ERROR_CODE(base, severity, index) (base | severity | index)

#define GET_SEVERITY(error) (error & 0xF000000000000000)
#define IS_SEVERITY(error, severity) (GET_SEVERITY(error) == severity)