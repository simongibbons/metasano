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
        stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(byte);
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

static const uint8_t from_base64[] =
{ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255,  62, 255,  63,
   52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,   0, 255, 255, 255,
  255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
   15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255,  63,
  255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
   41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 255, 255, 255, 255, 255
};

// TODO: While this works for any properly encoded base64 strings it does
// absolutely NO error checking / sanitizing....... this is a BAD THING.
std::vector<uint8_t> decode_base64(const std::string& s)
{
    std::vector<uint8_t> ret;
    const uint8_t *sarr = reinterpret_cast<const uint8_t*>(s.c_str());

    uint8_t block[3];

    for(size_t i = 0 ; i < (s.size() / 4) ; ++i) {
        std::fill(block, block+3, 0);
        block[0] = (from_base64[sarr[4*i + 0]] << 2) +
                   ((from_base64[sarr[4*i + 1]] & 0x30) >> 4);

        block[1] = ((from_base64[sarr[4*i + 1]] & 0xf) << 4 ) +
                   ((from_base64[sarr[4*i + 2]] & 0x3c) >> 2);

        block[2] = ((from_base64[sarr[4*i +2]] &0x3) << 6) +
                     from_base64[sarr[4*i+3]];

        for(size_t j = 0 ; j < 3 ; ++j) {
            ret.push_back(block[j]);
        }
    }

    //Trim any extras due to padding
    size_t pad_count = 0;
    for(auto rit = s.rbegin() ; *rit == '=' ; ++rit, pad_count++) {}

    ret.resize( ret.size() - pad_count);

    return ret;
}
