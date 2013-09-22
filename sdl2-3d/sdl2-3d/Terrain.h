#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <glm\glm.hpp>

typedef unsigned int GLuint;

class HeightMap;

class Terrain
{
public:
	Terrain(HeightMap& heightMap, float scale, float heightScale);
	~Terrain();

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
	unsigned int m_numLights;

	GLuint m_vao;

	glm::vec3 m_lightPos;
	bool m_followCam;

	GLuint m_shininess;
	GLuint m_strength;
	GLuint m_eyeDirection;

	//buffers
	GLuint m_indexBuffer;
	GLuint m_positionBuffer;
	GLuint m_texCoordBuffer;
	GLuint m_normalBuffer;

	GLuint* m_indices;
	glm::vec3* m_positions;
	glm::vec2* m_texCoords;
	glm::vec3* m_normals;
};

#endif //TERRAIN_H_