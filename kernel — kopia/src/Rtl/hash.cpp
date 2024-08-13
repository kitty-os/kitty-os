//
// Created by Piotr on 09.08.2024.
//

#include <xxHash/xxhash.h>
#include <cstddef>

namespace std
{
    std::size_t _Hash_bytes(void const* bytes, unsigned long len, unsigned long seed)
    {
        return XXH64(bytes, len, seed);
    }

    namespace __detail
    {
        class _Prime_rehash_policy
        {
        public:
            [[gnu::used]] bool _M_need_rehash(unsigned long current_size, unsigned long num_elements, unsigned long max_load_factor) const
            {
                double load_factor = static_cast<double>(num_elements) / current_size;

                return load_factor > max_load_factor;
            }

            [[gnu::used]] unsigned long _M_next_bkt(unsigned long current_bkt) const
            {
                // Assume we use a prime rehash policy, where bucket indices are based on a prime number
                // This is a simple example, actual implementation would be more complex

                // For demonstration, we'll just return the next bucket index
                // In a real scenario, this would be more complex, considering prime numbers and rehashing policy
                return current_bkt + 1; // Incrementing to the next bucket
            }
        };
    }
}