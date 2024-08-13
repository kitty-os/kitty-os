#pragma once

#include <cstdint>
#include <wchar.h>

#define DEBUG_PORT 0xE9
#define HEX_DIGITS "0123456789ABCDEF"

void DbgPrintChar(const char chr);
void DbgPrint(const char* msg);
void DbgPrintHexadecimal(uint64_t hexadecimal);
void DbgPrintf(const char* fmt, ...);

void DbgPrintWchar(const wchar_t wchr);
void DbgWprintf(const wchar_t* fmt, ...);