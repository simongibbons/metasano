#include <util/aes.h>
#include <util/padding.h>
#include <util/encoding.h>

#include <iostream>
#include <algorithm>
#include <map>
#include <random>

std::random_device rd;
std::uniform_int_distribution<uint32_t> dist(0, 200);

std::vector<uint8_t> prefix = generate_random_key(dist(rd));
std::vector<uint8_t> key = generate_random_key(16);

std::vector<uint8_t> make_ctext(const std::string& s)
{
    std::vector<uint8_t> unknown_string = decode_base64(
        "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg"
        "aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq"
        "dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg"
        "YnkK");

    std::vector<uint8_t> ptext(prefix.begin(), prefix.end());
    ptext.insert(ptext.end(), s.begin(), s.end());
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
    std::cout << "Block Size of Cipher is: " << block_size << std::endl;
    ////////////////////////////////////////////////////////////////////

    // Detect that cipher is using ECB mode
    ctext = make_ctext(std::string(block_size*3, 'a'));

    std::vector< std::vector<uint8_t> > blocks;
    for(size_t i = 0 ; i < ctext.size()/block_size ; ++i ) {
        blocks.emplace_back(ctext.begin() + i*block_size,
                            ctext.begin() + (i+1)*block_size);
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

    //////// Find the length of the random prefix

    //Firstly find the number of whole blocks we are offset by
    size_t start_block = 0;
    while(blocks[start_block] != blocks[start_block+1]) {
        start_block++;
    }

    //Now find the length of the remaining bytes
    auto test_block = blocks[start_block];

    size_t offset = 0;
    while(true) {
        ctext = make_ctext( std::string(block_size + offset, 'a') );
        std::vector<uint8_t> trial_block(ctext.begin() + block_size*start_block,
                                         ctext.begin() + block_size*(start_block+1));
        if( trial_block == test_block ) break;
        offset++;
    }

    size_t prefix_size = (start_block - 1)*block_size + (16 - offset);

    std::cout << "Random Prefix is " << prefix_size << " bytes long" << std::endl;

    ///////////////////////////////////////////////////////////////////////////////

    // Now we can decrypt as we did in problem 12

    auto make_ctext_as_p12 = [=](const std::string &s){
        std::string ptext(offset, 'a');
        ptext += s;
        auto ctext = make_ctext(ptext);
        return std::vector<uint8_t>(ctext.begin() + start_block*block_size, ctext.end());
    };

    unknown_size = unknown_size - start_block*block_size;

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

            ctext = make_ctext_as_p12(input_block);
            std::vector<uint8_t> ctext_block(ctext.begin() + j*block_size,
                                             ctext.begin() + (j+1)*block_size);

            for(uint8_t c = 0 ; c < 255 ; ++c ) {
                std::string trial_block = input_block + recovered;
                trial_block += c;
                ctext = make_ctext_as_p12(trial_block);
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
    std::cout << ptext << std::endl;



    return 0;
}
