#ifndef PERLINGENERATOR_H_
#define PERLINGENERATOR_H_

#include "Random.h"
#include "MathUtils.h"

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


	Random random;
	float gradients[GRADIENTS_TABLE_SIZE * 3];
};

#endif //PERLINGENERATOR_H_