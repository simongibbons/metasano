#pragma once

#include <vector>
#include <cstdint>

#include <exception>
#include <stdexcept>

class InvalidPadding : public std::runtime_error {
public:
    InvalidPadding() : runtime_error("Invalid Padding")
    {}
};

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

template <typename Container>
Container pkcs7_unpad(const Container &input)
{
    size_t expected_pad = input.back();
    size_t actual_pad = 0;
    for(auto rit = input.rbegin() ; rit != input.rend() ; ++rit) {
        if(*rit == expected_pad) {
            actual_pad++;
        } else {
            break;
        }
    }

    if(expected_pad > actual_pad) {
        throw InvalidPadding();
    }

    return Container(input.begin(), input.end() - actual_pad);
}
