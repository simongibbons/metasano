#include <util/encoding.h>
#include <iostream>

int main()
{
    std::string hex_str = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

    std::cout << encode_base64(decode_hex(hex_str)) << std::endl;
}
