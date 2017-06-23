// Endless Online Awaken v0.0.1

#ifndef RANDGEN_HPP_INCLUDED
#define RANDGEN_HPP_INCLUDED

#include <chrono>
#include <random>

class RandomGenerator
{
private:
    unsigned seed;
    std::mt19937 gen;

public:
    RandomGenerator();
    int RandInt(int imin, int imax);
};

#endif // RANDGEN_HPP_INCLUDED
