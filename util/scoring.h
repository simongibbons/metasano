#pragma once

#include <vector>
#include <cstdint>
#include <string>

double score_english( const std::vector<uint8_t>& bs);
double score_english( const std::string& s);

uint64_t hamming_distance(const std::vector<uint8_t>& s1, const std::vector<uint8_t> &s2);
