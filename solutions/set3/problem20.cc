#include <util/aes.h>
#include <util/encoding.h>
#include <util/scoring.h>
#include <util/xor.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

std::vector<uint8_t> key = generate_random_key(16);
uint64_t nonce = 1234535;

std::vector< std::vector<uint8_t> > load_ptexts()
{
    std::string fname;

    std::cout << "Enter Path to Datafile: " << std::endl;
    std::cin >> fname;

    std::ifstream fin;
    fin.open(fname);
    if( !fin.is_open() ) {
        throw std::runtime_error("Datafile not found");
    }

    std::vector< std::vector<uint8_t> > ptexts;
    std::string s;
    while( fin >> s ) {
        ptexts.push_back(decode_base64(s));
    }
    fin.close();
    return ptexts;
}

int main()
{
    auto ptexts = load_ptexts();
    std::vector< std::vector<uint8_t> > ctexts;
    for(auto &pt : ptexts) {
        ctexts.emplace_back( AES128_CTR_encrypt(pt, key, nonce) );
    }

    // Truncate ciphertexts to length of shortest element
    auto min_ctext = std::min_element(ctexts.begin(), ctexts.end(),
                      [](const std::vector<uint8_t> &v1,
                         const std::vector<uint8_t> &v2) 
                      {return v1.size() < v2.size();});
    size_t trunc_length = min_ctext->size();

    // Transpose ctexts to blocks xord with the same byte
    // of the effective stream cipher.
    std::vector< std::vector<uint8_t> > blocks(trunc_length);
    for(size_t i = 0 ; i < ctexts.size() ; ++i) {
        for(size_t j = 0 ; j < trunc_length ; ++j)
            blocks[j].push_back(ctexts[i][j]);
    }

    std::vector<uint8_t> key_stream;
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
        key_stream.push_back(key_character);
    }

    for(auto& ctext : ctexts) {
        auto recovered_ptext = xor_buffers(ctext, key_stream);
        std::cout << std::string(recovered_ptext.begin(), recovered_ptext.end())
                  << std::endl;
    }

    return 0;
}