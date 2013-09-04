#include "HeightMap.h"

/** ported from http://www.float4x4.net/index.php/2010/06/generating-realistic-and-playable-terrain-height-maps/ */

HeightMap::HeightMap(int seed, int width_, int height_)
	: width(width_)
	, height(height_)
	, perlin(seed)
{
	heights = new float[width * height];
	memset(heights, 0, width * height * sizeof(float));
}

HeightMap::~HeightMap()
{
	delete [] heights;
}

void HeightMap::addPerlinNoise(float f)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			heights[i * height + j] += perlin.noise(f * i / (float) width, f * j / (float) height, 0);
		}
	}
}

float HeightMap::getHeightAt(int x, int y)
{
	if (x < 0 || x > width || y < 0 || y > height) 
			return -0.01f;
	return heights[(int) x * height + (int) y];
	
}

void HeightMap::perturb(float f, float d)
{
	int u, v;
	float* temp = new float[width * height];

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			u = i + (int)(perlin.noise(f * i / (float) width, f * j / (float) height, 0) * d);
			v = j + (int)(perlin.noise(f * i / (float) width, f * j / (float) height, 1) * d);
			if (u < 0) u = 0; if (u >= width) u = width - 1;
			if (v < 0) v = 0; if (v >= height) v = height - 1;
			temp[i * height + j] = heights[u * height + v];
		}
	}
	memcpy(heights, temp, width * height * sizeof(float));

	delete [] temp;
}

void HeightMap::erode(float smoothness)
{
	for (int i = 1; i < width - 1; i++)
	{
		for (int j = 1; j < height - 1; j++)
		{
			float d_max = 0.0f;
			int match[2] = { 0, 0 };
 
			for (int u = -1; u <= 1; u++)
			{
				for (int v = -1; v <= 1; v++)
				{
					if(abs(u) + abs(v) > 0)
					{
						float d_i = heights[i * height + j] - heights[(i + u) * height +  (j + v)];
						if (d_i > d_max)
						{
							d_max = d_i;
							match[0] = u; match[1] = v;
						}
					}
				}
			}
 
			if(0 < d_max && d_max <= (smoothness / (float) width))
			{
				float d_h = 0.5f * d_max;
				heights[i * height + j] -= d_h;
				heights[(i + match[0]) * height + (j + match[1])] += d_h;
			}
		}
	}
}

void HeightMap::smoothen()
{
	for (int i = 1; i < width - 1; ++i)
	{
		for (int j = 1; j < height - 1; ++j)
		{
			float total = 0.0f;
			for (int u = -1; u <= 1; u++)
			{
				for (int v = -1; v <= 1; v++)
				{
					total += heights[(i + u) * height + (j + v)];
				}
			}
 
			heights[i * height + j] = total / 9.0f;
		}
	}
}