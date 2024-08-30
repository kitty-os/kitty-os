#include "mt19937.hpp"

// Define the Mersenne Twister parameters
static const int MT_SIZE = 624;
static const uint32_t MT_MASK = 0x80000000U;
static const uint32_t MT_UMASK = 0x7FFFFFFFU;
static const uint32_t MT_MAG02 = 0x9908B0DFU;
static const uint32_t MT_TEMPER_MASK = 0x9D2C5680U;
static const uint32_t MT_TEMPER_MASK2 = 0xEFC60000U;
static const int MT_SHIFT = 11;
static const int MT_SKEW = 7;
static const int MT_ROTOR = 15;

// Internal state and index
static uint32_t mt[MT_SIZE];
static int mt_index = MT_SIZE + 1;

static void init(uint32_t seed) {
    mt[0] = seed;
    for (unsigned int i = 1; i < MT_SIZE; ++i)
    {
        mt[i] = (1812433253U * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
    }
    mt_index = MT_SIZE;
}

static uint32_t gen(void)
{
    if (mt_index >= MT_SIZE)
    {
        int i;
        for (i = 0; i < MT_SIZE - 1; ++i)
        {
            uint32_t y = (mt[i] & MT_MASK) | (mt[i + 1] & MT_UMASK);
            mt[i] = mt[(i + 397) % MT_SIZE] ^ (y >> 1);
            if (y & 1) mt[i] ^= MT_MAG02;
        }
        uint32_t y = (mt[MT_SIZE - 1] & MT_MASK) | (mt[0] & MT_UMASK);
        mt[MT_SIZE - 1] = mt[396] ^ (y >> 1);
        if (y & 1) mt[MT_SIZE - 1] ^= MT_MAG02;
        mt_index = 0;
    }
    uint32_t y = mt[mt_index++];
    y ^= (y >> MT_SHIFT);
    y ^= (y << MT_SKEW) & MT_TEMPER_MASK;
    y ^= (y << MT_ROTOR) & MT_TEMPER_MASK2;
    y ^= (y >> 18);
    return y;
}

void CryptMT19937SetSeed(uint32_t seed)
{
    init(seed);
}

uint32_t CryptMT19937Generate(void)
{
    return gen();
}
