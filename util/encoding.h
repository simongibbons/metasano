#pragma once

#include <vector>
#include <string>
#include <cstdint>

std::vector<uint8_t> decode_hex(const std::string& s);
std::string encode_hex(const std::vector<uint8_t>& bs);

std::vector<uint8_t> decode_base64(const std::string& s);
std::string encode_base64(const std::vector<uint8_t>& bs);
