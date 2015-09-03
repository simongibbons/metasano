#include <util/aes.h>
#include <util/encoding.h>

#include <string>
#include <array>
#include <iostream>
#include <fstream>

int main()
{
    std::string key_str = "YELLOW SUBMARINE";
    std::vector<uint8_t> key(key_str.begin(), key_str.end());

    //Load the ctext
    std::string fname;

    std::cout << "Enter Path to Datafile: ";
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

    std::vector<uint8_t> ctext = decode_base64(ctext_b64);

    std::vector<uint8_t> ptext;

    for(size_t i = 0 ; i < ctext.size() / 16 ; ++i ) {
        std::vector<uint8_t> ctext_block(ctext.begin() + i*16, ctext.begin() + 16*(i+1));
        std::vector<uint8_t> ptext_block = AES128_decrypt_block(ctext_block, key);

        ptext.insert(ptext.end(), ptext_block.begin(), ptext_block.end());
    }

    std::string ptext_string(ptext.begin(), ptext.end());

    std::cout << "\nDecrypted Text:\n==================\n";
    std::cout << ptext_string << std::endl;

    return 0;
}
