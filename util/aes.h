#pragma once

#include <vector>
#include <cstdint>

std::vector<uint8_t> generate_random_key(uint64_t key_size);

std::vector<uint8_t> AES128_encrypt_block(const std::vector<uint8_t>& block,
                                          const std::vector<uint8_t>& key);

std::vector<uint8_t> AES128_decrypt_block(const std::vector<uint8_t>& block,
                                          const std::vector<uint8_t>& key);

std::vector<uint8_t> AES128_ECB_encrypt(const std::vector<uint8_t>& ptext,
                                        const std::vector<uint8_t>& key);

std::vector<uint8_t> AES128_ECB_decrypt(const std::vector<uint8_t>& ptext,
                                        const std::vector<uint8_t>& key);

std::vector<uint8_t> AES128_CBC_encrypt(const std::vector<uint8_t>& ptext,
                                        const std::vector<uint8_t>& key,
                                        const std::vector<uint8_t>& iv);

std::vector<uint8_t> AES128_CBC_decrypt(const std::vector<uint8_t>& ctext,
                                        const std::vector<uint8_t>& key,
                                        const std::vector<uint8_t>& iv);

std::vector<uint8_t> AES128_CTR_encrypt(const std::vector<uint8_t>& ptext,
                                        const std::vector<uint8_t>& key,
                                        const uint64_t nonce);

std::vector<uint8_t> AES128_CTR_decrypt(const std::vector<uint8_t>& ctext,
                                        const std::vector<uint8_t>& key,
                                        const uint64_t nonce);