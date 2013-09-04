#ifndef HEIGHTMAP_H_
#define HEIGHTMAP_H_

#include "PerlinGenerator.h"

class HeightMap
{
public:
	HeightMap(int seed, int width, int height);
	~HeightMap();

	void addPerlinNoise(float f);
	void perturb(float spikeyness, float mountainFrequency);
	void erode(float smoothness);
	void smoothen();

	float getHeightAt(int x, int y);

	int getWidth()
	{
		return width;
	}
	int getHeight()
	{
		return height;
	}
private:
	int width, height;

	float* heights;
	PerlinGenerator perlin;
};

#endif //HEIGHTMAP_H_