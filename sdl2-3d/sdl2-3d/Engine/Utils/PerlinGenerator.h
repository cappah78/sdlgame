#pragma once

#include "Random.h"

static const int GRADIENTS_TABLE_SIZE = 256;

class PerlinGenerator
{
public:
	PerlinGenerator(int seed);
	~PerlinGenerator();

	float noise(float x, float y, float z);

private:
	void initGradients();
	int permutate(int x);
	int index(int ix, int iy, int iz);
	float lattice(int ix, int iy, int iz, float fx, float fy, float fz);
	float lerp(float t, float val0, float val1);
	float smooth(float x);

	Random<float> m_random;
	float m_gradients[GRADIENTS_TABLE_SIZE * 3];
};