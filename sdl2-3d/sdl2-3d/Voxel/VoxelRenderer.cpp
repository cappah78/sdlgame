#include "VoxelRenderer.h"

#include "..\Engine\Graphics\Camera.h"
#include "..\Engine\Utils\ShaderManager.h"
#include <glm/gtx/transform.hpp>

const char* MVP_UNIFORM_NAME = "u_mvp";
const char* NORMAL_UNIFORM_NAME = "u_normal";

const unsigned int INDEX_MASK = 0x00000FFFu;
const unsigned int TEXTURE_ID_MASK = 0x00FFF000u;

static const GLfloat g_texCoordData[] = {
	1.0, 0.0,
	0.0, 0.0,
	1.0, 1.0,
	0.0, 1.0
};
// face, corner, vertices

static const int g_faceVertexOffsetsInt[6][4][3] = {
	{ //top
		{ 1, 1, 0 },
		{ 0, 1, 0 },
		{ 1, 1, 1 },
		{ 0, 1, 1 }
	},
	{ //bottom
		{ 1, 0, 1 },
		{ 0, 0, 1 },
		{ 1, 0, 0 },
		{ 0, 0, 0 }
	},
	{ //left
		{ 0, 1, 0 },
		{ 1, 1, 0 },
		{ 0, 0, 0 },
		{ 1, 0, 0 }
	},
	{ //right
		{ 1, 1, 1 },
		{ 0, 1, 1 },
		{ 1, 0, 1 },
		{ 0, 0, 1 }
	},
	{ //front
		{ 1, 1, 0 },
		{ 1, 1, 1 },
		{ 1, 0, 0 },
		{ 1, 0, 1 }
	},
	{ //back
		{ 0, 1, 1 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
		{ 0, 0, 0 }
	}
};

static const GLfloat g_faceVertexData [6][4][3] = {
	{	//top
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f }
	},
	{	//bottom
		{ 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f } 
	},
	{	//left
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	},
	{	//right
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f }
	},
	{	//front
		{ 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f }
	},
	{	//back
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f }
	}
};

VoxelRenderer::VoxelRenderer()
	: m_begunRender(false)
	, m_begunCache(false)
	, m_blendEnabled(false)
	, m_texcoordBuffer(MAX_FACES_PER_CACHE * 4)
	, m_indiceBuffer(MAX_FACES_PER_CACHE * 6, GL_ELEMENT_ARRAY_BUFFER)
{
	m_pointData.resize(MAX_FACES_PER_CACHE * 4);
	m_colorData.resize(MAX_FACES_PER_CACHE * 4);

	m_shaderId = ShaderManager::createShaderProgram("Assets/Shaders/voxelshaderinstanced.vert", NULL, "Assets/Shaders/voxelshaderinstanced.frag");
	
	glBindVertexArray(0);
	glUseProgram(m_shaderId);

	m_mvpUniformLoc = glGetUniformLocation(m_shaderId, MVP_UNIFORM_NAME);
	m_normalUniformLoc = glGetUniformLocation(m_shaderId, MVP_UNIFORM_NAME);

	for (int i = 0; i < MAX_FACES_PER_CACHE; ++i)
	{
		m_texcoordBuffer.add(glm::vec2(1.0f, 0.0f));
		m_texcoordBuffer.add(glm::vec2(0.0f, 0.0f));
		m_texcoordBuffer.add(glm::vec2(1.0f, 1.0f));
		m_texcoordBuffer.add(glm::vec2(0.0f, 1.0f));
	}
	m_texcoordBuffer.update();

	unsigned short j = 0;
	for (int i = 0; i < MAX_FACES_PER_CACHE * 6; i += 6, j += 4)
	{
		m_indiceBuffer.add(j + 0);
		m_indiceBuffer.add(j + 1);
		m_indiceBuffer.add(j + 2);
		m_indiceBuffer.add(j + 1);
		m_indiceBuffer.add(j + 3);
		m_indiceBuffer.add(j + 2);
	}
	m_indiceBuffer.update();
}

void VoxelRenderer::beginRender(const TextureArray* tileSet)
{
	assert(!m_begunRender && "Call endRender() before beginRender()");
	m_begunRender = true;
	tileSet->bind();
	glUseProgram(m_shaderId);
}


void VoxelRenderer::renderChunk(Chunk* const chunk, const Camera& camera)
{
	assert(m_begunRender);
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix[3][0] = chunk->m_xOffset;
	modelMatrix[3][1] = chunk->m_yOffset;
	modelMatrix[3][2] = chunk->m_zOffset;

	glm::mat4& mvpMatrix = camera.m_combinedMatrix * modelMatrix;
	
	setMVPUniform(mvpMatrix);

	glBindVertexArray(chunk->m_vao);
	glDrawElements(GL_TRIANGLES, chunk->m_numFaces * 6, GL_UNSIGNED_SHORT, 0);
}

void VoxelRenderer::endRender()
{
	assert(m_begunRender && "Call beginRender() before endRender()");
	m_begunRender = false;
	glUseProgram(0);
}

VoxelRenderer::Chunk* const VoxelRenderer::createChunk(float xOffset, float yOffset, float zOffset)
{
	assert(!m_begunRender && "Cannot create a new chunk in between beginRender() and endRender()");

	Chunk* chunk = new Chunk(xOffset, yOffset, zOffset);
	glGenVertexArrays(1, &chunk->m_vao);
	glBindVertexArray(chunk->m_vao);

	m_indiceBuffer.bind();
	chunk->m_pointBuffer.setAttribPointer(0, GL_UNSIGNED_INT, 1, GL_FALSE, GL_TRUE);
	chunk->m_colorBuffer.setAttribPointer(COLOR_LOC, GL_UNSIGNED_BYTE, 4, GL_TRUE);
	m_texcoordBuffer.setAttribPointer(TEXCOORD_LOC, GL_FLOAT, 2);


	return chunk;
}

VoxelRenderer::~VoxelRenderer()
{
	glDeleteShader(m_shaderId);
}

void VoxelRenderer::beginChunk(Chunk* const chunk)
{
	assert(!chunk->m_begun && "This chunk has already begun");
	assert(!m_begunCache && "A chunk has already begun");
	m_begunCache = true;

	chunk->m_numFaces = 0;

	chunk->m_pointBuffer.reset();
	chunk->m_colorBuffer.reset();

	chunk->m_pointBuffer.setBackingArray(&m_pointData[0], m_pointData.size() * sizeof(m_pointData[0]));
	chunk->m_colorBuffer.setBackingArray(&m_colorData[0], m_colorData.size() * sizeof(m_colorData[0]));

	chunk->m_begun = true;
}

void VoxelRenderer::Chunk::addFace(Face face, int x, int y, int z, int textureID, Color8888 color1, Color8888 color2, Color8888 color3, Color8888 color4)
{
	assert(m_begun && "Chunk has not yet begun");

	m_numFaces++;

	for (int i = 0; i < 4; ++i)
	{
		int xOffset = g_faceVertexOffsetsInt[face][i][0];
		int yOffset = g_faceVertexOffsetsInt[face][i][1];
		int zOffset = g_faceVertexOffsetsInt[face][i][2];

		VoxelVertex vertex(x + xOffset, y + yOffset, z + zOffset, textureID);
		m_pointBuffer.add(vertex);
	}

	m_colorBuffer.add(color1);
	m_colorBuffer.add(color2);
	m_colorBuffer.add(color3);
	m_colorBuffer.add(color4);
	//std::cout << (int) color1.r << ":" << (int) color1.g << ":" << (int) color1.b << ":" << (int) color1.a << std::endl;
}

void VoxelRenderer::endChunk(Chunk* const chunk)
{
	assert(m_begunCache && chunk->m_begun && "Cache has not yet begun");
	m_begunCache = false;

	chunk->m_pointBuffer.update();
	chunk->m_colorBuffer.update();

	chunk->m_pointBuffer.clearBackingArray();
	chunk->m_colorBuffer.clearBackingArray();

	chunk->m_begun = false;
}

void VoxelRenderer::setMVPUniform(glm::mat4 mvpMatrix)
{
	assert(m_begunRender);
	glUniformMatrix4fv(m_mvpUniformLoc, 1, GL_FALSE, &mvpMatrix[0][0]);
}
void VoxelRenderer::setNormalUniform(glm::vec3 normal)
{
	assert(m_begunRender);
	glUniform3fv(m_normalUniformLoc, 1, &normal[0]);
}

void VoxelRenderer::deleteChunk(Chunk* chunk)
{
	glDeleteVertexArrays(1, &chunk->m_vao);
	delete chunk;
	chunk = NULL;
}