#pragma once

#include <vector>
#include <cstdint>

std::vector<uint8_t> AES128_encrypt_block(const std::vector<uint8_t>& block,
                                          const std::vector<uint8_t>& key);

std::vector<uint8_t> AES128_decrypt_block(const std::vector<uint8_t>& block,
                                          const std::vector<uint8_t>& key);

std::vector<uint8_t> AES128_CBC_encrypt(const std::vector<uint8_t>& ptext,
                                        const std::vector<uint8_t>& key,
                                        const std::vector<uint8_t>& iv);

std::vector<uint8_t> AES128_CBC_decrypt(const std::vector<uint8_t>& ctext,
                                        const std::vector<uint8_t>& key,
                                        const std::vector<uint8_t>& iv);
