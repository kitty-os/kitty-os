#pragma once

#include <cstdint>

#define DEBUG_PORT 0xE9
#define HEX_DIGITS "0123456789ABCDEF"

void DbgPrintChar(const char chr);
void DbgPrint(const char* msg);
void DbgPrintHexadecimal(uint64_t hexadecimal);