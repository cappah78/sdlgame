#include "Terrain.h"

#include <sstream>
#include "HeightMap.h"
#include "Camera.h"
#include <gl\glew.h>


const int IN_POSITION_LOC = 0;
const int IN_TEXCOORD_LOC = 1;
const int IN_NORMAL_LOC = 2;

#define BUFFER_OFFSET(offset) ((void *)(offset))
#define m_terrainVERTEX_SHADER "terrain.vert"
#define m_terrainFRAGMENT_SHADER "terrain.frag"
#define m_terrainUNI_AMBIENT "ambient"
#define m_terrainUNI_LIGHTCOLOR "lightColor"
#define m_terrainUNI_LIGHTPOSITION "lightPosition"
#define m_terrainUNI_SHININESS "shininess"
#define m_terrainUNI_STRENGTH "strength"
#define m_terrainUNI_EYEDIRECTION "eyeDirection"
#define m_terrainUNI_CONSTATTEN "constantAttenuation"
#define m_terrainUNI_LINATTEN "linearAttenuation"
#define m_terrainUNI_QUADATTEN "quadraticAttenuation"


Terrain::Terrain(HeightMap& heightMap, float scale, float heightScale)
	: m_scale(scale)
	, m_heightScale(heightScale)
	, m_lightPos(0, 10, 0)
	, m_followCam(true)
	, m_numLights(10)
{
	m_width = heightMap.getWidth();
	m_height = heightMap.getHeight();
	m_numVertices = m_width * m_height;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	generateIndices();
	generateVertices(heightMap);
	generateNormals();
}

Terrain::~Terrain()
{
	delete [] m_positions;
	delete [] m_indices;
	delete [] m_texCoords;
	delete [] m_normals;
}

void Terrain::generateIndices()
{
	m_numIndices = (m_width - 1) * (m_height - 1) * 6;

	m_indices = new GLuint[m_numIndices];

	unsigned int idx = 0;
	for (unsigned int i = 0; i < m_width - 1; ++i)
	{
		for (unsigned int j = 0; j < m_height - 1; ++j)
		{
			unsigned int vertexIndex = j * m_width + i;

			m_indices[idx++] = vertexIndex;
			m_indices[idx++] = vertexIndex + m_width + 1;
			m_indices[idx++] = vertexIndex + 1;
			m_indices[idx++] = vertexIndex;
			m_indices[idx++] = vertexIndex + m_width;
			m_indices[idx++] = vertexIndex + m_width + 1;
		}
	}

	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), m_indices, GL_STATIC_DRAW);
}

void Terrain::generateVertices(HeightMap& heightMap)
{
	float terrainWidth = m_width * m_scale;
	float terrainHeight = m_height * m_scale;

	float halfTerrainWidth = terrainWidth * 0.5f;
	float halfTerrainHeight = terrainHeight * 0.5f; 

	m_positions = new glm::vec3[m_numVertices];
	m_texCoords = new glm::vec2[m_numVertices];

	unsigned int idx = 0;
	// height first so counter clockwise vertices
	for (unsigned int j = 0; j < m_height; ++j) {
		for (unsigned int i = 0; i < m_width; ++i) {

			float heightValue = heightMap.getHeightAt(i, j);

			//percentages of total width/height
			float s = (i / (float) (m_width - 1));
			float t = (j / (float) (m_height - 1));

			float x = (s * terrainWidth) - halfTerrainWidth;
			float y = heightValue * m_heightScale;
			float z = (t * terrainHeight) - halfTerrainHeight;

			m_positions[idx] = glm::vec3(x, y, z);
			m_texCoords[idx] = glm::vec2(s, t);

			idx++;
		}
	}

	glGenBuffers(1, &m_positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), m_positions, GL_STATIC_DRAW);
	glVertexAttribPointer(IN_POSITION_LOC, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	
	glGenBuffers(1, &m_texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec2), m_texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(IN_TEXCOORD_LOC, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
}

void Terrain::generateNormals() 
{
	m_normals = new glm::vec3[m_numVertices];
	for (unsigned int i = 0; i < m_numIndices; i += 3) 
	{
		glm::vec3 v0 = m_positions[m_indices[i]];
		glm::vec3 v1 = m_positions[m_indices[i + 1]];
		glm::vec3 v2 = m_positions[m_indices[i + 2]];
			
		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		m_normals[m_indices[i]] += normal;
		m_normals[m_indices[i + 1]] += normal;
		m_normals[m_indices[i + 2]] += normal;
	}

	//const glm::vec3 up(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_numVertices; ++i)
	{
		m_normals[i] = glm::normalize(m_normals[i]);
	}
		
	glGenBuffers(1, &m_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), m_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(IN_NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);
}

void Terrain::render()
{
	//glEnable(GL_DEPTH_TEST);
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_width * m_height * 6, GL_UNSIGNED_INT, 0);
}

float Terrain::getHeightAt(const glm::vec3& position)
{
	float result = 0;

	float terrainWidth = m_width * m_scale;
	float terrainHeight = m_height * m_scale;
	float halfTerrainWidth = terrainWidth * 0.5f;
	float halfTerrainHeight = terrainHeight * 0.5f;

	glm::vec3 terrainPos = glm::vec3(position);
	glm::vec3 offset(halfTerrainWidth, 0.0f, halfTerrainHeight);
	glm::vec3 vertexIndices = ( terrainPos + offset ) * (1.0f / m_scale);

	int u0 = (int)floorf(vertexIndices.x);
    int u1 = u0 + 1;
    int v0 = (int)floorf(vertexIndices.z);
    int v1 = v0 + 1;

	if ( u0 >= 0 && u1 < (int) m_width && v0 >= 0 && v1 < (int) m_height )
    {                    
        glm::vec3 p00 = m_positions[ (v0 * m_width) + u0 ];    // Top-left vertex
        glm::vec3 p10 = m_positions[ (v0 * m_width) + u1 ];    // Top-right vertex
        glm::vec3 p01 = m_positions[ (v1 * m_width) + u0 ];    // Bottom-left vertex
        glm::vec3 p11 = m_positions[ (v1 * m_width) + u1 ];    // Bottom-right vertex

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