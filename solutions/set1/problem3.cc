#include <util/xor.h>
#include <util/encoding.h>
#include <util/scoring.h>
#include <iostream>

int main()
{
    std::string ctext("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");

    std::vector<uint8_t> ctext_bs = decode_hex(ctext);

    double maxscore = -100000000000000.0;
    std::vector<uint8_t> best_key;
    std::vector<uint8_t> best_ptext_bs;

    for(uint8_t i = 0 ; i < 255 ; ++i) {
        std::vector<uint8_t> key{i};

        auto ptext_bs = xor_repeating(ctext_bs, key);

        double score = score_english(ptext_bs);
        if(score > maxscore) {
            maxscore = score;
            best_key = key;
            best_ptext_bs = ptext_bs;
        }

    }

    std::string ptext(best_ptext_bs.begin(), best_ptext_bs.end());

    std::cout << "Recovered Plaintext: " << ptext << std::endl;
    std::cout << "Recovered Key: " << best_key[0] << std::endl;
    std::cout << "Score: " << maxscore << std::endl;

    return 0;
}
