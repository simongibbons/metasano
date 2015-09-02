#include <util/xor.h>
#include <util/encoding.h>

#include <iostream>

int main()
{
    std::vector<uint8_t> key{'I','C','E'};
    std::string ptext = "Burning 'em, if you ain't quick and nimble\n"
                        "I go crazy when I hear a cymbal";

    std::vector<uint8_t> ptext_bs(ptext.begin(), ptext.end());
    std::vector<uint8_t> ctext_bs = xor_repeating(ptext_bs, key);

    std::cout << encode_hex(ctext_bs) << std::endl;

    return 0;
}

