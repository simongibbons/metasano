#include <util/aes.h>
#include <util/padding.h>
#include <util/encoding.h>

#include <iostream>
#include <algorithm>
#include <map>

std::vector<uint8_t> key = generate_random_key(16);

std::vector<uint8_t> make_ctext(const std::string& s)
{
    std::vector<uint8_t> unknown_string = decode_base64(
        "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg"
        "aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq"
        "dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg"
        "YnkK");

    std::vector<uint8_t> ptext(s.begin(), s.end());
    ptext.insert(ptext.end(), unknown_string.begin(), unknown_string.end());

    return AES128_ECB_encrypt(pkcs7_pad(ptext, 16), key);
}

int main()
{
    // Determine block size of cipher
    auto ctext = make_ctext("");
    size_t unknown_size = ctext.size();
    size_t extra = 0;
    do {
        ctext = make_ctext(std::string(++extra, 'a'));
    } while(ctext.size() == unknown_size);

    size_t block_size = ctext.size() - unknown_size;
    std::cout << "Length of Unknown String is: " << unknown_size << std::endl;
    std::cout << "Block Size of Cipher is: " << block_size << std::endl;
    ////////////////////////////////////////////////////////////////////


    // Detect that cipher is using ECB mode
    ctext = make_ctext(std::string(96, 'a'));

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
        std::cout << "Unknown block cipher mode" << std::endl;
    }
    ////////////////////////////////////////////////////////////////////

    // Do the byte at a time decryption attack

    std::vector< std::string > ptext_blocks;
    for(size_t j = 0 ; j < unknown_size / block_size ; ++j) { //For each block
        std::string recovered = "";
        for(size_t i = 0 ; i < block_size ; ++i ) {
            std::string input_block;

            // If we are recovering the first block then we can use arbitary
            // input of the correct length - otherwise we need to use the
            // previous block of recovered plaintext.
            if(ptext_blocks.size() == 0 ) {
                input_block = std::string(block_size - i - 1, 'a');
            } else {
                input_block = std::string(ptext_blocks[j-1].begin() + i + 1,
                                          ptext_blocks[j-1].end());
            }

            ctext = make_ctext(input_block);
            std::vector<uint8_t> ctext_block(ctext.begin() + j*block_size,
                                             ctext.begin() + (j+1)*block_size);

            for(uint8_t c = 0 ; c < 255 ; ++c ) {
                std::string trial_block = input_block + recovered;
                trial_block += c;
                ctext = make_ctext(trial_block);
                std::vector<uint8_t> block(ctext.begin(),
                                           ctext.begin() + block_size );
                if(block == ctext_block) {
                    recovered += c;
                    break;
                }
            }
        }
        ptext_blocks.push_back(recovered);
    }

    std::cout << "Recovered Plaintext:" << std::endl;

    std::string ptext;
    for(auto& pt : ptext_blocks) {
        ptext += pt;
    }
    std::cout << pkcs7_unpad(ptext) << std::endl;


    return 0;
}
