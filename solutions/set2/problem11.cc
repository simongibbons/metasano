#include <util/aes.h>
#include <util/padding.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>

std::vector<uint8_t> key = generate_random_key(16);

std::random_device rd;
std::uniform_int_distribution<uint8_t> coin_flip(0, 1);
std::uniform_int_distribution<uint8_t> extra_padding(5, 10);

std::vector<uint8_t> generate_ctext(std::string s)
{
    uint8_t mode = coin_flip(rd);

    std::vector<uint8_t> ptext = generate_random_key( extra_padding(rd) );
    ptext.insert(ptext.end(), s.begin(), s.end());

    std::vector<uint8_t> suffix = generate_random_key( extra_padding(rd) );
    ptext.insert(ptext.end(), suffix.begin(), suffix.end());

    switch(mode) {
    case 0:
        std::cout << "Encrypting with: ECB" << std::endl;
        return AES128_ECB_encrypt(pkcs7_pad(ptext, 16), key);
    case 1:
        std::cout << "Encrypting with: CBC" << std::endl;
        std::vector<uint8_t> iv = generate_random_key(16);
        return AES128_CBC_encrypt(pkcs7_pad(ptext, 16), key, iv);
    }
    throw std::runtime_error("Invalid Mode Selected");
}

int main()
{
    auto ctext = generate_ctext(std::string(128, 'a'));

    std::vector< std::vector<uint8_t> > blocks;
    for(size_t i = 0 ; i < ctext.size()/32 ; ++i ) {
        blocks.emplace_back(ctext.begin() + i*32,
                            ctext.begin() + (i+1)*32);
    } 

    uint64_t count = 0;
    for(auto& block : blocks) {
        count += std::count(blocks.begin(), blocks.end(), block) - 1;
    }

    if(count > 0) {
        std::cout << "Detected: ECB mode" << std::endl;
    } else {
        std::cout << "Detected: CBC mode" << std::endl;
    }

    return 0;
}