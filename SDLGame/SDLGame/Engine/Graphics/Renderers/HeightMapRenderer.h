#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <glm\glm.hpp>

class HeightMap;

/** 
Renders a heightmap using the following vertex attributes locations:

position			0 vec3
texture coordinate	1 vec2
normal				2 vec3

Does not manage the shader used to render.
*/
class HeightMapRenderer
{
public:
	typedef unsigned int GLuint;

	static const unsigned int IN_POSITION_LOC = 0;	//vec3
	static const unsigned int IN_TEXCOORD_LOC = 1;	//vec2
	static const unsigned int IN_NORMAL_LOC = 2;	//vec3

	HeightMapRenderer(HeightMap& heightMap, float scale, float heightScale);
	~HeightMapRenderer();

	void render();
	float getHeightAt(const glm::vec3& position);

private:
	void generateIndices();
	void generateVertices(HeightMap& heightMap);
	void generateNormals();

	unsigned int m_width;
	unsigned int m_height;
	float m_scale;
	float m_heightScale;

	unsigned int m_numVertices;
	unsigned int m_numIndices;

	GLuint m_vao;
	enum Buffers { INDICES, POSITION, TEXCOORD, NORMAL, NUM_BUFFERS };
	GLuint m_buffers[NUM_BUFFERS];

	unsigned int* m_indices;
	glm::vec3* m_positions;
	glm::vec2* m_texCoords;
	glm::vec3* m_normals;
};

#endif //TERRAIN_H_