// Endless Online Awaken v0.0.1

#include "randgen.hpp"

RandomGenerator::RandomGenerator()
{
    this->seed = std::chrono::system_clock::now().time_since_epoch().count();
    this->gen = std::mt19937(seed);
}

int RandomGenerator::RandInt(int imin, int imax)
{
    std::uniform_int_distribution<> uniform_dist(imin, imax);
    int rand_num = uniform_dist(this->gen);

    return rand_num;
}
