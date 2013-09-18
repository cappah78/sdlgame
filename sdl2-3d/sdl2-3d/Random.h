#ifndef RANDOM_H_
#define RANDOM_H_

#include <random>
#include <iostream>
#include <stdio.h>

class Random
{
public:
	Random(int seed, float minVal, float maxVal)
		: distribution(minVal, maxVal)
    {
		engine.seed(seed);
		distribution.reset();
    }
 
    ~Random()
    {

    }
 
    float next()
    {
		return distribution(engine);
    }
private:
	typedef std::tr1::ranlux64_base_01 Engine;
	typedef std::tr1::uniform_real_distribution<float> Distribution;

	Engine engine;
	Distribution distribution;
};

#endif //RANDOM_H_
