#include <util/scoring.h>
#include <util/encoding.h>
#include <util/xor.h>

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

    // Determine the most likely value for the block size.
    double min_distance = 100000000000;
    size_t block_size = 2;
    for(size_t trial = 2 ; trial <= 40 ; ++trial) {
        std::vector< std::vector<uint8_t> > blocks;
        for(size_t i = 0 ; i < 4 ; ++i) {
            blocks.emplace_back(ctext.begin() + i*trial,
                                ctext.begin() + (i+1)*trial);
        }

        double distance = 0.0;
        for(size_t i = 0 ; i < blocks.size() ; ++i) {
            for(size_t j = i ; j < blocks.size() ; ++j) {
                distance += hamming_distance(blocks[i], blocks[j]);
            }
        }
        distance /= trial;

        if(distance < min_distance) {
            block_size = trial;
            min_distance = distance;
        }
    }

    std::cout << "Block Size = " << block_size << std::endl;


    // Transpose ciphertext into blocks each encrypted with the
    // same character

    std::vector< std::vector<uint8_t> > blocks(block_size);
    for(size_t i = 0 ; i < ctext.size() ; ++i) {
        blocks[i % block_size].push_back(ctext[i]);
    }

    // Determine the character that was used to encrypt each block.
    std::vector<uint8_t> key;
    for(auto& block : blocks) {
        double best_score = -100000.0;
        uint8_t key_character = 'a';

        for(uint8_t i = 0; i < 255 ; ++i) {
            double score = score_english(xor_repeating(block, {i}));
            if(score > best_score) {
                best_score = score;
                key_character = i;
            }
        }
        key.push_back(key_character);
    }

    std::cout << "Recovered Key: " << std::string(key.begin(), key.end()) << std::endl;

    auto ptext = xor_repeating(ctext, key);

    std::cout << "Recovered Plaintext: \n";
    std::cout << std::string(ptext.begin(), ptext.end()) << std::endl;
 
    return 0;
}