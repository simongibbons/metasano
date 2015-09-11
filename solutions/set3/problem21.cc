#include <util/mt19937.h>

#include <iostream>
#include <random>

int main()
{
    mt19937 rng(42);

    std::mt19937 reference_rng(42);
    std::uniform_int_distribution<uint32_t> dist(0, 0xffffffff);

    std::cout << "Me\t\tReference\n";
    std::cout << "==========================\n";
    for(size_t i = 0 ; i < 20 ; ++i) {
        std::cout << rng.next() << "\t" << dist(reference_rng) << std::endl;
    }

    return 0;
}