#pragma once

#include <vector>
#include <cstdint>

template <typename Container>
Container pkcs7_pad(const Container& input, uint8_t block_size)
{
    uint8_t pad_length = block_size - input.size() % block_size;

    if( pad_length == 0 ) {
        pad_length = block_size;
    }

    Container ret = input;

    for(uint8_t i = 0 ; i < pad_length ; ++i ) {
        ret.push_back(pad_length);
    }

    return ret;
}

