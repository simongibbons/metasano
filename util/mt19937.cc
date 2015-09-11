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
        uint32_t y = (state[i] & 0x80000000) +
                     (state[(i + 1) % n] & 0x7fffffff);
        state[i] = state[(i + m) % n] ^ y >> 1;

        if(y % 2 != 0)
            state[i] = state[i] ^ a;
    }
}

void mt19937::print_state()
{
    for(uint32_t i = 0 ; i < n ; ++i) {
        std::cout << state[i] << " ";
    }
    std::cout << std::endl;
}