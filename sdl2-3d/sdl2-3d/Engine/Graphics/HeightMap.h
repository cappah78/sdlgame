#ifndef HEIGHTMAP_H_
#define HEIGHTMAP_H_

#include "..\Utils\PerlinGenerator.h"

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
		return m_width;
	}
	int getHeight()
	{
		return m_height;
	}
private:
	int m_width;
	int m_height;

	float* m_heights;
	PerlinGenerator m_perlin;
};

#endif //HEIGHTMAP_H_