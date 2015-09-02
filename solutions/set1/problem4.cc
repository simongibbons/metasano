#include <util/xor.h>
#include <util/encoding.h>
#include <util/scoring.h>
#include <iostream>
#include <fstream>

int main()
{
    std::string fname;

    std::cout << "Enter Path to Datafile: ";
    std::cin >> fname;

    std::ifstream fin;
    fin.open(fname);
    if( !fin.is_open() ) {
        std::cerr << "Cannot Find Datafile\n";
        return 2;
    }

    std::vector< std::string > ctexts;
    std::string s;
    while( fin >> s ) {
        ctexts.push_back(s);
    }

    double best_score = -10000000.0;
    std::string best_ctext;
    std::string best_ptext;
    char best_key = 'a';

    for(auto ctext : ctexts) {
        auto ctext_bs = decode_hex(ctext);
        for(uint8_t i = 0 ; i < 255 ; ++i) {
            std::vector<uint8_t> key{i};

            auto ptext_bs = xor_repeating(ctext_bs, key);
            double score = score_english(ptext_bs);

            if(score > best_score) {
                best_score = score;
                best_key = i;
                best_ptext = std::string(ptext_bs.begin(), ptext_bs.end());
                best_ctext = ctext;
            }
        }
    }

    std::cout << "Most Likely ctext: " << best_ctext << std::endl;
    std::cout << "Plaintext: " << best_ptext << std::endl;
    std::cout << "Key: " << best_key << std::endl;
    std::cout << "Score: " << best_score << std::endl;

    return 0;
}
