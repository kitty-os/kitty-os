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
        };
    }
}