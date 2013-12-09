#include "VoxelCache.h"

#include "..\Engine\Graphics\Camera.h"
#include "..\Engine\Graphics\Material.h"
#include "..\Engine\Utils\ShaderManager.h"
#include <glm/gtx/transform.hpp>

const unsigned int SIZE_BITS = 4; //2^sizebits == chunksize or sqrt(chunkSize)
const unsigned int INDEX_BITS = 3 * SIZE_BITS;
const unsigned int TEXTURE_ID_BITS = 12;
const unsigned int OCCLUSION_BITS = 8;

const char* CACHE_VERTEX_TRANSFORM_UNIFORM_NAME = "VoxelTransform2";
const int CACHE_VERTEX_TRANSFORM_BINDING_POINT = 0;

const unsigned int INDEX_MASK = 0x00000FFFu;
const unsigned int TEXTURE_ID_MASK = 0x00FFF000u;
const unsigned int LIGHT_LEVEL_MASK = 0xFF000000u;

static const GLfloat g_texCoordData[] = {
	1.0, 0.0,
	0.0, 0.0,
	1.0, 1.0,
	0.0, 1.0
};

static const GLfloat g_topFaceVertexData[] = {
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f
};

static const GLfloat g_bottomFaceVertexData[] = {
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f
};

static const GLfloat g_leftFaceVertexData[] = {
	0.0f, 1.0f, 0.0f,
	1.0, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f
};

static const GLfloat g_rightFaceVertexData[] = {
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

static const GLfloat g_frontFaceVertexData[] = {
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f
};

static const GLfloat g_backFaceVertexData[] = {
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f
};

VoxelCache::VoxelCache()
	: m_drawing(false)
	, m_blendEnabled(false)
{
	m_shaderId = ShaderManager::createShaderProgram("Assets/Shaders/voxelshaderinstanced.vert", NULL, "Assets/Shaders/voxelshaderinstanced.frag");
	
	glBindVertexArray(0);
	glUseProgram(m_shaderId);

	glGenBuffers(1, &m_cameraTransformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(m_cameraTransform), &m_cameraTransform, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, CACHE_VERTEX_TRANSFORM_BINDING_POINT, m_cameraTransformBuffer);

	GLuint vertexTransformIdx = glGetUniformBlockIndex(m_shaderId, CACHE_VERTEX_TRANSFORM_UNIFORM_NAME);
	glUniformBlockBinding(m_shaderId, vertexTransformIdx, CACHE_VERTEX_TRANSFORM_BINDING_POINT);

	glGenBuffers(1, &m_texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_texCoordData), g_texCoordData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_topFaceVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_topFaceVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_topFaceVertexData), g_topFaceVertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &m_bottomFaceVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_bottomFaceVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_bottomFaceVertexData), g_bottomFaceVertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &m_leftFaceVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_leftFaceVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_leftFaceVertexData), g_leftFaceVertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &m_rightFaceVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_rightFaceVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_rightFaceVertexData), g_rightFaceVertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &m_frontFaceVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_frontFaceVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_frontFaceVertexData), g_frontFaceVertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &m_backFaceVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_backFaceVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_backFaceVertexData), g_backFaceVertexData, GL_STATIC_DRAW);
}

void VoxelCache::Cache::addFace(int x, int y, int z, int textureID, float color1, float color2, float color3, float color4)
{
	assert(m_size > (m_pointIdx + 1) && "Amount of draw calls exceeded size");

	// position+texId+occlusionbits are being packed in one int here
	unsigned int index = x | (y | z << SIZE_BITS) << SIZE_BITS;
	unsigned int vertexData = index;
	vertexData |= textureID << INDEX_BITS;;
	m_points[m_pointIdx++] = vertexData;
}


void VoxelCache::beginRender()
{
	assert(!m_drawing && "Call end() before begin()");
	m_drawing = true;
	glUseProgram(m_shaderId);
}

void VoxelCache::finishRender()
{
	assert(m_drawing && "Call begin() before end()");
	m_drawing = false;
	glUseProgram(0);
}

void VoxelCache::renderCache(Cache* const cache, const TextureArray* tileSet, const Camera& camera)
{
	assert(m_drawing);
	if (cache->m_size == 0)
		return;

	glBindVertexArray(cache->m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, cache->m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, cache->m_size * sizeof(int), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, cache->m_pointIdx * sizeof(int), cache->m_points);

	setUniforms(camera, cache->m_face, cache->m_xOffset, cache->m_yOffset, cache->m_zOffset);
	tileSet->bind();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, cache->m_pointIdx);
}

glm::mat4 modelMatrix = glm::mat4(1);

void VoxelCache::setUniforms(const Camera& camera, Face face, float xOffset, float yOffset, float zOffset)
{
	modelMatrix[3][0] = xOffset * 0.5f;	//TODO: figure out why / 2
	modelMatrix[3][1] = yOffset * 0.5f;
	modelMatrix[3][2] = zOffset * 0.5f;
	m_cameraTransform.matrix = camera.m_combinedMatrix * modelMatrix;

	switch (face)	//TODO: verify if normals are correct
	{
	case TOP:	 m_cameraTransform.normal = glm::vec3(0.0, 1.0, 0.0);	break;
	case BOTTOM: m_cameraTransform.normal = glm::vec3(0.0, -1.0, 0.0);	break;
	case LEFT:	 m_cameraTransform.normal = glm::vec3(-1.0, 0.0, 0.0);	break;
	case RIGHT:  m_cameraTransform.normal = glm::vec3(1.0, 0.0, 0.0);	break;
	case FRONT:  m_cameraTransform.normal = glm::vec3(0.0, 0.0, 1.0);	break;
	case BACK:	 m_cameraTransform.normal = glm::vec3(0.0, 0.0, -1.0);	break;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(VoxelTransform), &m_cameraTransform, GL_STATIC_DRAW);
}

void VoxelCache::deleteCache(Cache* const cache)
{
	//TODO: clean up all the buffers!!
	glDeleteBuffers(1, &cache->m_positionBuffer);
	glDeleteVertexArrays(1, &cache->m_vao);
	delete cache;
}

VoxelCache::Cache* const VoxelCache::createCache(Face face, unsigned int size, float xOffset, float yOffset, float zOffset)
{
	assert(!m_drawing && "Cannot create a new cache in between begin() and end()");

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//reuse constant buffers from voxelcache
	switch (face)
	{
	case TOP:	glBindBuffer(GL_ARRAY_BUFFER, m_topFaceVertexBuffer);	break;
	case BOTTOM:glBindBuffer(GL_ARRAY_BUFFER, m_bottomFaceVertexBuffer);break;
	case LEFT:	glBindBuffer(GL_ARRAY_BUFFER, m_leftFaceVertexBuffer);	break;
	case RIGHT: glBindBuffer(GL_ARRAY_BUFFER, m_rightFaceVertexBuffer); break;
	case FRONT: glBindBuffer(GL_ARRAY_BUFFER, m_frontFaceVertexBuffer); break;
	case BACK:	glBindBuffer(GL_ARRAY_BUFFER, m_backFaceVertexBuffer);	break;
	}
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	//create position buffer unique to cache
	GLuint positionBuffer;
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(int), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(int), 0);

	glVertexAttribDivisor(0, 0); // vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 0); // texcoords : always reuse the same 4 texcoords -> 0
	glVertexAttribDivisor(2, 1); // position : one per quad (its center) -> 1

	return new Cache(vao, size, positionBuffer, face, xOffset, yOffset, zOffset);
}

VoxelCache::~VoxelCache()
{
	glDeleteShader(m_shaderId);
}
