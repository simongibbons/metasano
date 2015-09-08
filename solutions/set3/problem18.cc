#include <util/aes.h>
#include <util/encoding.h>

#include <iostream>

int main()
{
    std::vector<uint8_t> ctext = decode_base64("L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==");
    std::vector<uint8_t> key = {'Y','E','L','L','O','W',' ','S','U','B','M','A','R','I','N','E'};
    uint64_t nonce = 0;

    std::vector<uint8_t> ptext = AES128_CTR_decrypt(ctext, key, nonce);

    std::cout << std::string(ptext.begin(), ptext.end()) << std::endl;
}