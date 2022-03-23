#include "Utils.hpp"
#include <random>

namespace Utils 
{
	unsigned int getRandomNumber(unsigned int min, unsigned int max)
	{
        std::random_device rnd;
        std::mt19937 generate(rnd());
        std::uniform_int_distribution<> distribute(min, max);
        return distribute(generate);
    }
}
