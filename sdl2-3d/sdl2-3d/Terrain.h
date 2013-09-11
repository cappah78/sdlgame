#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "KeyListener.h"

#include <glm\glm.hpp>

typedef unsigned int GLuint;

class HeightMap;

class Terrain : public KeyListener
{
public:
	Terrain(HeightMap& heightMap, float scale, float heightScale);
	~Terrain();

	bool keyDown(SDL_Keysym key);
	bool keyUp(SDL_Keysym key) { return false; };

	void render();
	float getHeightAt(const glm::vec3& position);

private:
	void generateIndices();
	void generateVertices(HeightMap& heightMap);
	void generateNormals();

	unsigned int width;
	unsigned int height;
	float scale;
	float heightScale;

	unsigned int numVertices;
	unsigned int numIndices;
	unsigned int numLights;

	GLuint vao;

	glm::vec3 lightPos;
	bool followCam;

	GLuint shininess;
	GLuint strength;
	GLuint eyeDirection;

	//buffers
	GLuint indexBuffer;
	GLuint positionBuffer;
	GLuint texCoordBuffer;
	GLuint normalBuffer;

	GLuint* indices;
	glm::vec3* positions;
	glm::vec2* texCoords;
	glm::vec3* normals;
};

#endif //TERRAIN_H_