#include "HeightMap.h"

/** ported from http://www.float4x4.net/index.php/2010/06/generating-realistic-and-playable-terrain-height-maps/ */

HeightMap::HeightMap(int seed, int width, int height)
	: m_width(width)
	, m_height(height)
	, m_perlin(seed)
{
	m_heights = new float[width * height];
	memset(m_heights, 0, width * height * sizeof(float));
}

HeightMap::~HeightMap()
{
	delete [] m_heights;
}

void HeightMap::addPerlinNoise(float f)
{
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			m_heights[i * m_height + j] += m_perlin.noise(f * i / (float) m_width, f * j / (float) m_height, 0);
		}
	}
}

float HeightMap::getHeightAt(int x, int y)
{
	if (x < 0 || x > m_width || y < 0 || y > m_height) 
			return -0.01f;
	return m_heights[(int) x * m_height + (int) y];
	
}

void HeightMap::perturb(float f, float d)
{
	int u, v;
	float* temp = new float[m_width * m_height];

	for (int i = 0; i < m_width; ++i)
	{
		for (int j = 0; j < m_height; ++j)
		{
			u = i + (int)(m_perlin.noise(f * i / (float) m_width, f * j / (float) m_height, 0) * d);
			v = j + (int)(m_perlin.noise(f * i / (float) m_width, f * j / (float) m_height, 1) * d);
			if (u < 0) u = 0; if (u >= m_width) u = m_width - 1;
			if (v < 0) v = 0; if (v >= m_height) v = m_height - 1;
			temp[i * m_height + j] = m_heights[u * m_height + v];
		}
	}
	memcpy(m_heights, temp, m_width * m_height * sizeof(float));

	delete [] temp;
}

void HeightMap::erode(float smoothness)
{
	for (int i = 1; i < m_width - 1; i++)
	{
		for (int j = 1; j < m_height - 1; j++)
		{
			float d_max = 0.0f;
			int match[2] = { 0, 0 };
 
			for (int u = -1; u <= 1; u++)
			{
				for (int v = -1; v <= 1; v++)
				{
					if(abs(u) + abs(v) > 0)
					{
						float d_i = m_heights[i * m_height + j] - m_heights[(i + u) * m_height +  (j + v)];
						if (d_i > d_max)
						{
							d_max = d_i;
							match[0] = u; match[1] = v;
						}
					}
				}
			}
 
			if(0 < d_max && d_max <= (smoothness / (float) m_width))
			{
				float d_h = 0.5f * d_max;
				m_heights[i * m_height + j] -= d_h;
				m_heights[(i + match[0]) * m_height + (j + match[1])] += d_h;
			}
		}
	}
}

void HeightMap::smoothen()
{
	for (int i = 1; i < m_width - 1; ++i)
	{
		for (int j = 1; j < m_height - 1; ++j)
		{
			float total = 0.0f;
			for (int u = -1; u <= 1; u++)
			{
				for (int v = -1; v <= 1; v++)
				{
					total += m_heights[(i + u) * m_height + (j + v)];
				}
			}
 
			m_heights[i * m_height + j] = total / 9.0f;
		}
	}
}