#define _GLIBCXX_USE_NANOSLEEP 
#include <util/mt19937.h>

#include <ctime>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <iostream>



// Generates random numbers from an mt19937 generator
// seeded with the current time.
uint32_t get_random_number()
{
    uint32_t seed = static_cast<uint32_t>(std::time(nullptr));
    mt19937 rng(seed);
    std::cout << "RNG seeded with: " << seed << std::endl;
    std::random_device rd;
    std::uniform_int_distribution<int> dist(40, 1000);

    std::this_thread::sleep_for ( std::chrono::seconds(dist(rd)) );

    return rng.next();
}


 
int main() 
{
    uint32_t random_number = get_random_number();

    uint32_t curr_time = static_cast<uint32_t>(std::time(nullptr));
    uint32_t seed;
    bool found_seed = false;

    for(uint32_t guess = curr_time - 1000 ; guess <= curr_time ; ++guess) {
        mt19937 rng(guess);

        if(random_number == rng.next()) {
            found_seed = true;
            seed = guess;
            break;
        }
    }

    if(found_seed)
        std::cout << "Recovered Seed = " << seed << std::endl;
    else
        std::cout << "Couldn't find seed of rng" << std::endl;

    return 0;
}