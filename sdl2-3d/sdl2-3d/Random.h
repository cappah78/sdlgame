#ifndef RANDOM_H_
#define RANDOM_H_

#include <random>

class Random
{
public:
	Random( int seed, float minVal, float maxVal )
    {
        gen = new Generator( Engine( seed ), Distribution( minVal, maxVal ) );
    }
 
    ~Random()
    {
        delete gen;
    }
 
    float next()
    {
        return (*gen)();
    }
private:
	typedef std::minstd_rand                             Engine;
	typedef std::uniform_real_distribution<float>        Distribution;
    typedef std::variate_generator<Engine, Distribution> Generator;
 
    Generator* gen;
};

#endif //RANDOM_H_
