#ifndef TERRAIN2_H_
#define TERRAIN2_H_

#include "HeightMap.h"
#include "Camera.h"
#include "Shader.h"

#include "KeyListener.h"

#include <gl\glew.h>
#include <glm\glm.hpp>


class Terrain2 : public KeyListener
{
public:
	Terrain2(HeightMap& heightMap, float scale, float heightScale);
	~Terrain2();

	bool keyDown(SDL_Keysym key);
	bool keyUp(SDL_Keysym key) { return false; };

	void render();
	float getHeightAt(const glm::vec3& position);

private:
	void generateIndices();
	void generateVertices(HeightMap& heightMap);
	void generateNormals();

	int width;
	int height;
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

#endif //TERRAIN2_H_