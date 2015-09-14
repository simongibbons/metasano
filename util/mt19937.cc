#include "mt19937.h"

#include <iostream>

mt19937::mt19937(uint32_t seed)
{
    reseed(seed);
}

uint32_t mt19937::next()
{
    if(index >= n) twist();

    uint32_t y = state[index++];
    y = y ^ ((y >> u) & d);
    y = y ^ ((y << s) & b);
    y = y ^ ((y << t) & c);
    y = y ^ (y >> l);

    return y;
}


void mt19937::reseed(uint32_t seed)
{
    index = n;
    state[0] = seed;
    for(uint32_t i = 1 ; i < n ; ++i) {
        state[i] = f * (state[i-1] ^ (state[i-1] >> (w-2))) + i;
    }
}

void mt19937::twist()
{
    index = 0;
    for(uint32_t i = 0 ; i < n ; ++i) {
        uint32_t y = (state[i] & 0x80000000) + //Upper 32 - r bits
                     (state[(i + 1) % n] & 0x7fffffff); //Lower r bits
        state[i] = state[(i + m) % n] ^ y >> 1;

        if(y % 2 != 0)
            state[i] = state[i] ^ a;
    }
}

void mt19937::set_state(std::array<uint32_t, n> new_state)
{
    state = new_state;
}

void mt19937::print_state()
{
    for(uint32_t i = 0 ; i < n ; ++i) {
        std::cout << state[i] << " ";
    }
    std::cout << std::endl;
}

std::vector<uint8_t> mt19937_encrypt(const std::vector<uint8_t>& ptext,
                                     const uint32_t seed)
{
    mt19937 rng(seed);
    
    uint32_t state;
    uint32_t mask = 0;

    std::vector<uint8_t> ctext;
    for(size_t i = 0 ; i < ptext.size() ; ++i) {
        if(mask == 0) {
            mask = 0x000000ff;
            state = rng.next();
        }
        ctext.push_back(ptext[i] ^ (state & mask) );
        mask <<= 1;
    }

    return ctext;
}

std::vector<uint8_t> mt19937_decrypt(const std::vector<uint8_t>& ctext,
                                     const uint32_t seed)
{
    return mt19937_encrypt(ctext, seed);
}
