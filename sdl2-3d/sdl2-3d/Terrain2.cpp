#include "Terrain2.h"

#include <sstream>

#define BUFFER_OFFSET(offset) ((void *)(offset))

#define TERRAIN_VERTEX_SHADER "terrain.vert"
#define TERRAIN_FRAGMENT_SHADER "terrain.frag"



#define TERRAIN_UNI_AMBIENT "ambient"
#define TERRAIN_UNI_LIGHTCOLOR "lightColor"
#define TERRAIN_UNI_LIGHTPOSITION "lightPosition"
#define TERRAIN_UNI_SHININESS "shininess"
#define TERRAIN_UNI_STRENGTH "strength"

#define TERRAIN_UNI_EYEDIRECTION "eyeDirection"
#define TERRAIN_UNI_CONSTATTEN "constantAttenuation"
#define TERRAIN_UNI_LINATTEN "linearAttenuation"
#define TERRAIN_UNI_QUADATTEN "quadraticAttenuation"

Terrain2::Terrain2(HeightMap& heightMap, float scale_, float heightScale_)
	: scale(scale_)
	, heightScale(heightScale_)
	, lightPos(0, 10, 0)
	, followCam(true)
	, numLights(10)
{
	width = heightMap.getWidth();
	height = heightMap.getHeight();
	numVertices = width * height;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	generateIndices();
	generateVertices(heightMap);
	generateNormals();
}

Terrain2::~Terrain2()
{
	delete [] positions;
	delete [] indices;
	delete [] texCoords;
	delete [] normals;
}

void Terrain2::generateIndices()
{
	numIndices = (width - 1) * (height - 1) * 6;

	indices = new GLuint[numIndices];

	unsigned int idx = 0;
	for (unsigned int i = 0; i < width - 1; ++i)
	{
		for (unsigned int j = 0; j < height - 1; ++j)
		{
			unsigned int vertexIndex = j * width + i;

			indices[idx++] = vertexIndex;
			indices[idx++] = vertexIndex + width + 1;
			indices[idx++] = vertexIndex + 1;
			indices[idx++] = vertexIndex;
			indices[idx++] = vertexIndex + width;
			indices[idx++] = vertexIndex + width + 1;
		}
	}

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void Terrain2::generateVertices(HeightMap& heightMap)
{
	float terrainWidth = width * scale;
	float terrainHeight = height * scale;

	float halfTerrainWidth = terrainWidth * 0.5f;
	float halfTerrainHeight = terrainHeight * 0.5f; 

	positions = new glm::vec3[numVertices];
	texCoords = new glm::vec2[numVertices];

	std::cout << "size: " << numVertices << std::endl;

	unsigned int idx = 0;
	// height first so counter clockwise vertices
	for (unsigned int j = 0; j < height; ++j) {
		for (unsigned int i = 0; i < width; ++i) {

			float heightValue = heightMap.getHeightAt(i, j);

			//percentages of total width/height
			float s = (i / (float) (width - 1));
			float t = (j / (float) (height - 1));

			float x = (s * terrainWidth) - halfTerrainWidth;
			float y = heightValue * heightScale;
			float z = (t * terrainHeight) - halfTerrainHeight;

			positions[idx] = glm::vec3(x, y, z);
			texCoords[idx] = glm::vec2(s, t);

			idx++;
		}
	}

	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::IN_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec2), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::IN_TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
}

void Terrain2::generateNormals() 
{
	normals = new glm::vec3[numVertices];
	for (unsigned int i = 0; i < numIndices; i += 3) 
	{
		glm::vec3 v0 = positions[indices[i]];
		glm::vec3 v1 = positions[indices[i + 1]];
		glm::vec3 v2 = positions[indices[i + 2]];
			
		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		normals[indices[i]] += normal;
		normals[indices[i + 1]] += normal;
		normals[indices[i + 2]] += normal;
	}

	//const glm::vec3 up(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < numVertices; ++i)
	{
		normals[i] = glm::normalize(normals[i]);
	}
		
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::IN_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);
}

void Terrain2::render()
{
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, width * height * 6, GL_UNSIGNED_INT, 0);
}

float Terrain2::getHeightAt(const glm::vec3& position)
{
	float result = 0;

	float terrainWidth = width * scale;
	float terrainHeight = height * scale;
	float halfTerrainWidth = terrainWidth * 0.5f;
	float halfTerrainHeight = terrainHeight * 0.5f;

	glm::vec3 terrainPos = glm::vec3(position);
	glm::vec3 offset(halfTerrainWidth, 0.0f, halfTerrainHeight);
	glm::vec3 vertexIndices = ( terrainPos + offset ) * (1.0f / scale);

	int u0 = (int)floorf(vertexIndices.x);
    int u1 = u0 + 1;
    int v0 = (int)floorf(vertexIndices.z);
    int v1 = v0 + 1;

	if ( u0 >= 0 && u1 < (int)width && v0 >= 0 && v1 < (int)height )
    {                    
        glm::vec3 p00 = positions[ (v0 * width) + u0 ];    // Top-left vertex
        glm::vec3 p10 = positions[ (v0 * width) + u1 ];    // Top-right vertex
        glm::vec3 p01 = positions[ (v1 * width) + u0 ];    // Bottom-left vertex
        glm::vec3 p11 = positions[ (v1 * width) + u1 ];    // Bottom-right vertex

        // Which triangle are we over?
        float percentU = vertexIndices.x - u0;
        float percentV = vertexIndices.z - v0;

        glm::vec3 dU, dV;
        if (percentU > percentV)
        {   // Top triangle
            dU = p10 - p00;
            dV = p11 - p10;
        }
        else
        {   // Bottom triangle
            dU = p11 - p01;
            dV = p01 - p00;
        }

        glm::vec3 heightPos = p00 + ( dU * percentU ) + ( dV * percentV );
        result = heightPos.y;
	}

	return result;
}

bool Terrain2::keyDown(SDL_Keysym key)
{
	switch(key.sym)
	{
	case SDLK_t:
		followCam = !followCam;
		break;
	case SDLK_r:
		followCam = false;
		lightPos.z -= 10;
		break;
	case SDLK_f:
		followCam = false;
		lightPos.z += 10;
		break;
	}

	return false;
}