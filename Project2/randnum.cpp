#include "randnum.h"

int randNum::randRange(int min, int max) {
    // Create a static random number generator
    static std::mt19937 generator(std::chrono::steady_clock::now().time_since_epoch().count());

    // Define the range of random numbers
    std::uniform_int_distribution<int> distribution(min, max);

    // Generate a random number
    return distribution(generator);
}