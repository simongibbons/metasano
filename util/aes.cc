#include "aes.h"
#include "xor.h"

#include <openssl/aes.h>
#include <iostream>
#include <random>

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

std::vector<uint8_t> AES128_ECB_encrypt(const std::vector<uint8_t>& ptext,
                                        const std::vector<uint8_t>& key)
{
    size_t nblocks = ptext.size() / 16;

    std::vector<uint8_t> ctext;
    ctext.resize(ptext.size());

    AES_KEY wctx;

    AES_set_encrypt_key(key.data(), 128, &wctx);

    for(size_t i = 0 ; i < nblocks ; ++i) {
        AES_encrypt(ptext.data() + 16*i, ctext.data() + 16*i, &wctx);
    }

    return ctext;
}

std::vector<uint8_t> AES128_ECB_decrypt(const std::vector<uint8_t>& ctext,
                                        const std::vector<uint8_t>& key)
{
    size_t nblocks = ctext.size() / 16;

    std::vector<uint8_t> ptext;
    ptext.resize(ctext.size());

    AES_KEY wctx;

    AES_set_decrypt_key(key.data(), 128, &wctx);

    for(size_t i = 0 ; i < nblocks ; ++i) {
        AES_decrypt(ctext.data() + 16*i, ptext.data() + 16*i, &wctx);
    }

    return ptext;
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

// I wouldn't trust this for an actual cryptograpic implimentation but
// this is good enough for a fairly non-deterministic key generator.
std::vector<uint8_t> generate_random_key(uint64_t key_size = 16)
{
    std::random_device rd;
    std::uniform_int_distribution<uint8_t> dist(0, 255);

    std::vector<uint8_t> key;

    for( uint64_t i = 0 ; i < key_size ; ++i ) {
        key.push_back(dist(rd));
    }

    return key;
}

class AES128_CTR_Keystream {
    // There are portability issues with this - it assumes a little endian
    // processor, as well as some UB with the use of a union to reinterpret
    // the two uint64_t as an array of bytes. YMMV (and check this if it)
    // becomes a source of bugs later........
    union {
        uint64_t counter[2];
        uint8_t bytes[16];
    } state;

    uint8_t block[16];
    uint8_t position;

    AES_KEY wctx;

public:
    AES128_CTR_Keystream(const std::vector<uint8_t> &key, const uint64_t nonce)
    {
        state.counter[0] = nonce;
        state.counter[1] = 0;
        AES_set_encrypt_key(key.data(), 128, &wctx);
        generate_next_block();
    }

    uint8_t next_byte() {
        if(position >= 16) generate_next_block();
        return block[position++];
    }

private:
    void generate_next_block() {
        AES_encrypt(state.bytes, block, &wctx);
        state.counter[1]++;
        position = 0;
    }

};

std::vector<uint8_t> AES128_CTR_encrypt(const std::vector<uint8_t>& ptext,
                                        const std::vector<uint8_t>& key,
                                        const uint64_t nonce)
{
    AES128_CTR_Keystream ks(key, nonce);

    std::vector<uint8_t> ctext;
    ctext.reserve(ptext.size());

    for(auto byte : ptext) {
        ctext.push_back(byte ^ ks.next_byte());
    }

    return ctext;
}

std::vector<uint8_t> AES128_CTR_decrypt(const std::vector<uint8_t>& ctext,
                                        const std::vector<uint8_t>& key,
                                        const uint64_t nonce)
{
    return AES128_CTR_encrypt(ctext, key, nonce);
}
