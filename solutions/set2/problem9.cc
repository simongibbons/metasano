#include <util/padding.h>
#include <util/encoding.h>

#include <string>
#include <iostream>

int main()
{
    std::string s = "YELLOW SUBMARINE";
    std::vector<uint8_t> s_bytes(s.begin(), s.end());
    unsigned int block_size = 20;

    std::string padded = pkcs7_pad(s, block_size);

    std::vector<uint8_t> padded_bytes(padded.begin(), padded.end());

    std::cout << "Input String: " << s << std::endl;
    std::cout << "Input String Hex Encoded:  " << encode_hex(s_bytes) << std::endl;
    std::cout << "Padded String Hex Encoded: " << encode_hex( padded_bytes ) << std::endl;
}
