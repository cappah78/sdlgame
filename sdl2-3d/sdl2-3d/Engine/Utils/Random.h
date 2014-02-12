#pragma once

#include <random>

template <typename T>
class Random
{
public:
	Random(unsigned long seed, T minVal, T maxVal)
		: m_distribution(minVal, maxVal)
    {
		m_engine.seed(seed);
		m_distribution.reset();
	};
 
	~Random() {};
 
    T next() { return m_distribution(m_engine); };
private:
	typedef std::tr1::ranlux64_base_01 Engine;
	typedef std::tr1::uniform_real_distribution<T> Distribution;

	Engine m_engine;
	Distribution m_distribution;
};
