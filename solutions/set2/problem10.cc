#include <util/encoding.h>
#include <util/aes.h>

#include <iostream>
#include <fstream>

int main()
{
    // Find the datafile and load the ciphertext
    std::string fname;

    std::cout << "Enter Path to Datafile:\n";
    std::cin >> fname;

    std::ifstream fin;
    fin.open(fname);
    if( !fin.is_open() ) {
        std::cerr << "Cannot Find Datafile\n";
        return 2;
    }

    std::string ctext_b64, s;
    while(fin >> s) {
        ctext_b64 += s;
    }
    auto ctext = decode_base64(ctext_b64);

    std::string key_str = "YELLOW SUBMARINE";
    std::vector<uint8_t> key(key_str.begin(), key_str.end());
    auto iv = decode_hex("00000000000000000000000000000000");

    auto ptext = AES128_CBC_decrypt(ctext, key, iv);

    std::cout << std::string(ptext.begin(), ptext.end()) << std::endl;
}