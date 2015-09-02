#include <string>
#include <vector>
#include <cstdint>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <locale>
#include <iomanip>

std::vector<uint8_t> decode_hex(const std::string& s)
{
    std::vector<uint8_t> ret_val;
    ret_val.reserve(s.length() / 2);

    for(size_t i = 0 ; i < s.length()/2 ; i++) {
        uint8_t byte;
        sscanf(&s[2*i], "%2hhx", &byte);
        ret_val.push_back(byte);
    }

    return ret_val;
}

std::string encode_hex(const std::vector<uint8_t>& bs)
{
    std::stringstream stream;
    for(auto byte : bs ) {
        stream << std::hex << static_cast<int>(byte);
    }
    return stream.str();
}

static const char to_base64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string encode_base64(const std::vector<uint8_t>& bs)
{
    std::string ret;

    uint8_t block_3[3];
    char block_4[4];

    // Firstly Encode all the full 3 byte blocks of the data
    for(size_t i = 0 ; i < bs.size()/3 ; ++i) {

        for(size_t j = 0 ; j < 3 ; ++j)
            block_3[j] = bs[3*i + j];

        block_4[0] = to_base64[ (block_3[0] & 0b11111100) >> 2 ];

        block_4[1] = to_base64[ ((block_3[0] & 0b00000011) << 4) +
                                ((block_3[1] & 0b11110000) >> 4) ];

        block_4[2] = to_base64[ ((block_3[1] & 0b00001111) << 2) +
                                ((block_3[2] & 0b11000000) >> 6) ];

        block_4[3] = to_base64[ (block_3[2] & 0b00111111) ];

        for(size_t j = 0 ; j < 4 ; ++j)
            ret += block_4[j];
    }

    //If there is no extra part of the string we are done.
    if(bs.size() % 3 == 0)
        return ret;

    //We want zeros where there is no data
    for(size_t i = 0 ; i < 3 ; ++i)
        block_3[i] = 0;

    for(size_t j = 0 ; j < bs.size() % 3 ; ++j)
        block_3[j] = bs[3*(bs.size()/3) + j];

    block_4[0] = to_base64[ (block_3[0] & 0b11111100) >> 2 ];

    block_4[1] = to_base64[ ((block_3[0] & 0b00000011) << 4) +
                            ((block_3[1] & 0b11110000) >> 4) ];

    block_4[2] = to_base64[ ((block_3[1] & 0b00001111) << 2) +
                            ((block_3[2] & 0b11000000) >> 6) ];

    //Add any extra characters
    for(size_t j = 0 ; j < bs.size() % 3 + 1; ++j)
        ret += block_4[j];

    //Add padding so the length of the resulting string is divisible by 4
    for(size_t i = 0 ; i < 3 - bs.size() % 3 ; ++i)
        ret += '=';

    return ret;
}


