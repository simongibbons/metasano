#include "aes.h"

#include <openssl/aes.h>
#include <iostream>

std::vector<uint8_t> AES128_encrypt_block(const std::vector<uint8_t>& block,
                                          const std::vector<uint8_t>& key)
{
    std::vector<uint8_t> out;
    out.resize(16);

    AES_KEY wctx;

    AES_set_encrypt_key(key.data(), 128, &wctx);
    AES_encrypt(block.data(), out.data(), &wctx);

    return out;
}

std::vector<uint8_t> AES128_decrypt_block(const std::vector<uint8_t>& block,
                                          const std::vector<uint8_t>& key)
{
    std::vector<uint8_t> out;
    out.resize(16);

    AES_KEY wctx;

    AES_set_decrypt_key(key.data(), 128, &wctx);
    AES_decrypt(block.data(), out.data(), &wctx);

    return out;
}

