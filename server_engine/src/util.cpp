// Endless Online Awaken

#include "util.hpp"

#include <algorithm>

bool RandGen::initialized_ = false;
unsigned int RandGen::seed;
std::mt19937 RandGen::gen;

unsigned int DecodeNumber(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4)
{
	if (b1 == 254) b1 = 1;
	if (b2 == 254) b2 = 1;
	if (b3 == 254) b3 = 1;
	if (b4 == 254) b4 = 1;

	if (b1 == 0) b1 = 128;
	if (b2 == 0) b2 = 128;
	if (b3 == 0) b3 = 128;
	if (b4 == 0) b4 = 128;

	--b1;
	--b2;
	--b3;
	--b4;

	return (b4*MAX3 + b3*MAX2 + b2*MAX1 + b1);
}

std::array<unsigned char, 4> EncodeNumber(unsigned int number)
{
	std::size_t throwaway;

	return EncodeNumber(number, throwaway);
}

std::array<unsigned char, 4> EncodeNumber(unsigned int number, std::size_t &size)
{
	std::array<unsigned char, 4> bytes { 254, 254, 254, 254 };
	unsigned int onumber = number;

	if (onumber >= MAX3)
	{
		bytes[3] = number / MAX3 + 1;
		number = number % MAX3;
	}

	if (onumber >= MAX2)
	{
		bytes[2] = number / MAX2 + 1;
		number = number % MAX2;
	}

	if (onumber >= MAX1)
	{
		bytes[1] = number / MAX1 + 1;
		number = number % MAX1;
	}

	bytes[0] = number + 1;

	for (int i = 3; i >= 0; --i)
	{
		if (i == 0)
		{
			size = 1;
			break;
		}
		else if (bytes[i] > 0)
		{
			size = i + 1;
			break;
		}
	}

	return bytes;
}

RandGen::RandGen()
{
    if(!this->initialized_)
    {
        this->seed = std::chrono::system_clock::now().time_since_epoch().count();
        this->gen = std::mt19937(seed);

        this->initialized_ = true;
    }
}

int RandGen::RandInt(int imin, int imax)
{
    std::uniform_int_distribution<> uniform_dist(imin, imax);
    int rand_num = uniform_dist(this->gen);

    return rand_num;
}

int path_length(int x1, int y1, int x2, int y2)
{
	int dx = std::abs(x1 - x2);
	int dy = std::abs(y1 - y2);

	return dx + dy;
}
