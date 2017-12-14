// Endless Online Awaken

#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED

#include <vector>
#include <array>
#include <chrono>
#include <random>

static const unsigned int MAX1 = 253;
static const unsigned int MAX2 = 64009;
static const unsigned int MAX3 = 16194277;

inline static std::vector<std::string> GetArgs(std::string str)
{
    std::vector<std::string> args;
    std::string word;
    for(std::size_t i = 0; i < str.length(); ++i)
    {
        if((str[i] == ' ' || i == str.length() - 1))
        {
            if(str[i] != ' ') word += str[i];
            if(!word.empty()) args.push_back(word);
            word.clear();
        }
        else if(str[i] != ' ')
        {
            word += str[i];
        }
    }

    return args;
}

unsigned int DecodeNumber(unsigned char, unsigned char = 254, unsigned char = 254, unsigned char = 254);
std::array<unsigned char, 4> EncodeNumber(unsigned int);
std::array<unsigned char, 4> EncodeNumber(unsigned int, std::size_t &size);

class RandGen
{
private:
    static bool initialized_;
    static unsigned int seed;
    static std::mt19937 gen;

public:
    RandGen();
    int RandInt(int imin, int imax);
};

#endif // UTIL_HPP_INCLUDED
