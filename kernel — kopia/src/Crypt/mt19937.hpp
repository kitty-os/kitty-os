#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the Mersenne Twister with a seed
void CryptMT19937SetSeed(uint32_t seed);

// Generate a 32-bit random value
uint32_t CryptMT19937Generate(void);

#ifdef __cplusplus
}
#endif
