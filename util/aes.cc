#include "aes.h"
#include "xor.h"

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

std::vector<uint8_t> AES128_CBC_encrypt(const std::vector<uint8_t>& ptext,
                                        const std::vector<uint8_t>& key,
                                        const std::vector<uint8_t>& iv)
{
    size_t nblocks = ptext.size() / 16;

    std::vector<uint8_t> ctext;
    ctext.reserve(nblocks*16);

    AES_KEY wctx;

    AES_set_encrypt_key(key.data(), 128, &wctx);

    std::vector<uint8_t> ctext_block = iv;

    for(size_t i = 0 ; i < nblocks ; ++i) {
        std::vector<uint8_t> ptext_block(ptext.begin() + 16*i,
                                         ptext.begin() + 16*(i+1));

        ptext_block = xor_buffers(ctext_block, ptext_block);

        AES_encrypt(ptext_block.data(), ctext_block.data(), &wctx);

        ctext.insert(ctext.end(), ctext_block.begin(), ctext_block.end());
    }

    return ctext;
}

std::vector<uint8_t> AES128_CBC_decrypt(const std::vector<uint8_t>& ctext,
                                        const std::vector<uint8_t>& key,
                                        const std::vector<uint8_t>& iv)
{
    size_t nblocks = ctext.size() / 16;

    std::vector<uint8_t> ptext;
    ptext.reserve(nblocks*16);

    AES_KEY wctx;

    AES_set_decrypt_key(key.data(), 128, &wctx);

    std::vector<uint8_t> prev_block = iv;

    for(size_t i = 0 ; i < nblocks ; ++i) {
        std::vector<uint8_t> ctext_block(ctext.begin() + 16*i,
                                         ctext.begin() + 16*(i+1));

        std::vector<uint8_t> ptext_block(16);

        AES_decrypt(ctext_block.data(), ptext_block.data(), &wctx);

        ptext_block = xor_buffers(ptext_block, prev_block);
        ptext.insert(ptext.end(), ptext_block.begin(), ptext_block.end());
        prev_block = ctext_block;
    }

    return ptext;
}
