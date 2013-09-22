#ifndef RANDOM_H_
#define RANDOM_H_

#include <random>
#include <iostream>
#include <stdio.h>

class Random
{
public:
	Random(int seed, float minVal, float maxVal)
		: m_distribution(minVal, maxVal)
    {
		m_engine.seed(seed);
		m_distribution.reset();
    }
 
    ~Random()
    {

    }
 
    float next()
    {
		return m_distribution(m_engine);
    }
private:
	typedef std::tr1::ranlux64_base_01 Engine;
	typedef std::tr1::uniform_real_distribution<float> Distribution;

	Engine m_engine;
	Distribution m_distribution;
};

#endif //RANDOM_H_
