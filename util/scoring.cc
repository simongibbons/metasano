#include "scoring.h"

#include <unordered_map>
#include <cctype>

double score_english(const std::vector<uint8_t> &bs)
{
    std::string s(bs.begin(), bs.end());
    return score_english(s);
}

static const std::unordered_map<char, double> english_letter_scores
{
 {'a', 0.08167},
 {'b', 0.01492},
 {'c', 0.02782},
 {'d', 0.04253},
 {'e', 0.12702},
 {'f', 0.02228},
 {'g', 0.02015},
 {'h', 0.06094},
 {'i', 0.06966},
 {'j', 0.00153},
 {'k', 0.00772},
 {'l', 0.04025},
 {'m', 0.02406},
 {'n', 0.06749},
 {'o', 0.07507},
 {'p', 0.01929},
 {'q', 0.00095},
 {'r', 0.05987},
 {'s', 0.06327},
 {'t', 0.09056},
 {'u', 0.02758},
 {'v', 0.00978},
 {'w', 0.02360},
 {'x', 0.00150},
 {'y', 0.01974},
 {'z', 0.00074},
 {' ', 0.12702},
 {'0', 0.00000},
 {'1', 0.00000},
 {'2', 0.00000},
 {'3', 0.00000},
 {'4', 0.00000},
 {'5', 0.00000},
 {'6', 0.00000},
 {'7', 0.00000},
 {'8', 0.00000},
 {'9', 0.00000},
 {'\'', 0.0000},
 {'"', 0.00000},
 {'!', 0.00000},
 {'?', 0.00000},
 {',', 0.00000},
 {'.', 0.00000},
 {'\t', 0.00000},
 {'\n', 0.00000}
};

double score_english( const std::string& s)
{
    double score = 0.0;
    for( auto c : s ) {
        auto c_score_it = english_letter_scores.find(std::tolower(c));
        if(c_score_it != english_letter_scores.end() )
            score += c_score_it->second;
        else
            score -= 0.1;
    }
    return score;
}

uint64_t hamming_distance(const std::vector<uint8_t>& s1, const std::vector<uint8_t> &s2)
{
    if(s1.size() != s2.size()) {
        throw "ERROR: hamming distance undefined for different length strings";
    }

    uint64_t dist = 0;

    for( size_t i = 0 ; i < s1.size() ; ++i ) {
        uint8_t diff = s1[i] ^ s2[i];
        while( diff > 0 ) {
            dist += diff % 2;
            diff = diff >> 1;
        }
    }

    return dist;
}
