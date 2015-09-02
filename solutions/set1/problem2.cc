#include <util/xor.h>
#include <util/encoding.h>

#include <iostream>
#include <string>

int main()
{
    std::string hex_str_1 = "1c0111001f010100061a024b53535009181c";
    std::string hex_str_2 = "686974207468652062756c6c277320657965";

    std::vector<uint8_t> xor_result = xor_buffers( decode_hex(hex_str_1),
                                                   decode_hex(hex_str_2) );

    std::cout << encode_hex( xor_result ) << std::endl;
}

