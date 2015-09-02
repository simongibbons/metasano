#include "xor.h"

#include <cstddef>

std::vector<uint8_t> xor_buffers(const std::vector<uint8_t>& b1,
                                 const std::vector<uint8_t>& b2)
{
    size_t len = std::min(b1.size(), b2.size());

    std::vector<uint8_t> ret;
    ret.reserve(len);

    for(size_t i = 0 ; i < len ; ++i) {
        ret.push_back( b1[i] ^ b2[i] );
    }

    return ret;
}

std::vector<uint8_t> xor_repeating(const std::vector<uint8_t> &large,
                                   const std::vector<uint8_t> &small)
{
    std::vector<uint8_t> ret;
    ret.reserve(large.size());

    for(size_t i = 0 ; i < large.size() ; ++i) {
        ret.push_back( large[i] ^ small[i % small.size()] );
    }

    return ret;
}

