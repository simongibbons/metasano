#pragma once

#include <cstdint>
#include <array>
#include <vector>

class mt19937 {
    static const uint32_t w = 32, n = 624, m = 397, r = 31;
    static const uint32_t a = 0x9908b0df;
    static const uint32_t u = 11, d = 0xffffffff;
    static const uint32_t s = 7, b = 0x9d2c5680;
    static const uint32_t t = 15, c = 0xefc60000;
    static const uint32_t l = 18;
    static const uint32_t f = 1812433253;

    uint32_t index;
    std::array<uint32_t, n> state;

public:
    mt19937(uint32_t seed);
    void reseed(uint32_t seed);
    uint32_t next();

    void print_state();
    void set_state(std::array<uint32_t, n> state);

private:
    void twist();
};

std::vector<uint8_t> mt19937_encrypt(const std::vector<uint8_t>& ptext,
                                     const uint32_t seed);

std::vector<uint8_t> mt19937_decrypt(const std::vector<uint8_t>& ctext,
                                     const uint32_t seed);
