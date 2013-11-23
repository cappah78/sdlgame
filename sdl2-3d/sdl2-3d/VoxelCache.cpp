#include "VoxelCache.h"

#include "Camera.h"
#include "Material.h"
#include "ShaderManager.h"
#include <glm/gtx/transform.hpp>

const unsigned int SIZE_BITS = 4; //2^sizebits == chunksize or sqrt(chunkSize)
const unsigned int INDEX_BITS = 3 * SIZE_BITS;
const unsigned int TEXTURE_ID_BITS = 12;
const unsigned int OCCLUSION_BITS = 8;

const char* CACHE_VERTEX_TRANSFORM_UNIFORM_NAME = "CameraTransform";
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

static const GLuint g_cornerData [] = {
	VoxelCache::AO_UP_RIGHT << (TEXTURE_ID_BITS + INDEX_BITS),
	VoxelCache::AO_UP_LEFT << (TEXTURE_ID_BITS + INDEX_BITS),
	VoxelCache::AO_DOWN_RIGHT << (TEXTURE_ID_BITS + INDEX_BITS),
	VoxelCache::AO_DOWN_LEFT << (TEXTURE_ID_BITS + INDEX_BITS)
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


VoxelCache::VoxelCache(unsigned int sizeInFaces)
	: m_sizeInFaces(sizeInFaces)
	, m_drawing(false)
	, m_buildingCache(false)
	, m_blendEnabled(false)
	, m_pointIdx(0)
{
	m_shaderId = ShaderManager::createShaderProgram("voxelshaderinstanced.vert", NULL, "voxelshaderinstanced.frag");
	
	glUseProgram(m_shaderId);

	m_points = new unsigned int[sizeInFaces];
	memset(m_points, 0, sizeInFaces * sizeof(int));

	glGenBuffers(1, &m_cameraTransformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, CACHE_VERTEX_TRANSFORM_BINDING_POINT, m_cameraTransformBuffer);

	GLuint vertexTransformIdx = glGetUniformBlockIndex(m_shaderId, CACHE_VERTEX_TRANSFORM_UNIFORM_NAME);
	glUniformBlockBinding(m_shaderId, vertexTransformIdx, CACHE_VERTEX_TRANSFORM_BINDING_POINT);
}

void VoxelCache::beginCache(Face face, float xOffset, float yOffset, float zOffset)
{
	assert(!m_buildingCache && "Call end() before begin()");
	m_buildingCache = true;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	switch (face)
	{
	case TOP:	 glBufferData(GL_ARRAY_BUFFER, sizeof(g_topFaceVertexData), g_topFaceVertexData, GL_STATIC_DRAW);		break;
	case BOTTOM: glBufferData(GL_ARRAY_BUFFER, sizeof(g_bottomFaceVertexData), g_bottomFaceVertexData, GL_STATIC_DRAW);	break;
	case LEFT:	 glBufferData(GL_ARRAY_BUFFER, sizeof(g_leftFaceVertexData), g_leftFaceVertexData, GL_STATIC_DRAW);		break;
	case RIGHT:  glBufferData(GL_ARRAY_BUFFER, sizeof(g_rightFaceVertexData), g_rightFaceVertexData, GL_STATIC_DRAW);	break;
	case FRONT:  glBufferData(GL_ARRAY_BUFFER, sizeof(g_frontFaceVertexData), g_frontFaceVertexData, GL_STATIC_DRAW);	break;
	case BACK:	 glBufferData(GL_ARRAY_BUFFER, sizeof(g_backFaceVertexData), g_backFaceVertexData, GL_STATIC_DRAW);		break;
	}
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glGenBuffers(1, &m_cornerIndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_cornerIndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cornerData), g_cornerData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(int), 0);

	glGenBuffers(1, &m_texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_texCoordData), g_texCoordData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glGenBuffers(1, &m_positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_sizeInFaces * sizeof(int), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(int), 0);

	glVertexAttribDivisor(0, 0); // vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 0); // corner nr : always reuse the same 4 values -> 0
	glVertexAttribDivisor(2, 0); // texcoords : always reuse the same 4 texcoords -> 0
	glVertexAttribDivisor(3, 1); // position : one per quad (its center) -> 1

	m_currentCache = new Cache(vao, m_positionBuffer, face, xOffset, yOffset, zOffset);

	glUseProgram(m_shaderId);
}

void VoxelCache::addFace(int x, int y, int z, int textureID, unsigned char occlusionBits)
{
	assert(m_buildingCache && "Call beginCache() before addFace()");

	assert(m_currentCache->m_amount < m_sizeInFaces && "Amount of draw calls exceeded size");
	m_currentCache->m_amount++;

	// position+texId+occlusionbits are being packed in one int here
	unsigned int index = x | (y | z << SIZE_BITS) << SIZE_BITS;
	unsigned int vertexData = occlusionBits << (TEXTURE_ID_BITS + INDEX_BITS);
	vertexData |= textureID << INDEX_BITS;
	vertexData |= index;
	m_points[m_pointIdx++] = vertexData;
}

VoxelCache::Cache* const VoxelCache::endCache()
{
	assert(m_buildingCache && "Call begin() before end()");
	m_buildingCache = false;

	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_sizeInFaces * sizeof(int), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_pointIdx * sizeof(int), m_points);

	glUseProgram(0);
	m_pointIdx = 0;

	Cache* const currentCache = m_currentCache;
	m_currentCache = NULL;

	return currentCache;
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
	if (cache->m_amount == 0)
		return;

	glBindVertexArray(cache->m_vao);
	setUniforms(camera, cache->m_face, cache->m_xOffset, cache->m_yOffset, cache->m_zOffset);

	tileSet->bind();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, cache->m_amount);
}

void VoxelCache::setUniforms(const Camera& camera, Face face, float xOffset, float yOffset, float zOffset)
{
	m_cameraTransform.VPMatrix = camera.m_combinedMatrix;
	m_cameraTransform.VMatrix = camera.m_viewMatrix;
	m_cameraTransform.PMatrix = camera.m_projectionMatrix;

	glm::mat4 modelMatrix = glm::translate(xOffset / 2.0f, yOffset / 2.0f, zOffset / 2.0f);	//TODO: wtf?
	m_cameraTransform.VPMatrix *= modelMatrix;

	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraTransformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraTransform), &m_cameraTransform, GL_STATIC_DRAW);
}

void VoxelCache::deleteCache(Cache* const cache)
{
	glDeleteBuffers(1, &cache->m_pointBuffer);
	glDeleteVertexArrays(1, &cache->m_vao);
	delete cache;
}

void VoxelCache::beginCache(Cache* const cache, Face face, float xOffset, float yOffset, float zOffset)
{
	assert(!m_drawing && "Call end() before begin()");
	m_drawing = true;
	cache->m_amount = 0;
	cache->m_face = face;
	cache->m_xOffset = xOffset;
	cache->m_yOffset = yOffset;
	cache->m_zOffset = zOffset;
	m_currentCache = cache;

	glBindVertexArray(cache->m_vao);

	glUseProgram(m_shaderId);
}

VoxelCache::Cache* const VoxelCache::createCache(Face face, float xOffset, float yOffset, float zOffset)
{
	assert(!m_drawing && "Cannot create a new cache in between begin() and end()");

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_topFaceVertexData), NULL, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glGenBuffers(1, &m_cornerIndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_cornerIndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cornerData), g_cornerData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(int), 0);

	glGenBuffers(1, &m_texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_texCoordData), g_texCoordData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glGenBuffers(1, &m_positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_sizeInFaces * sizeof(int), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(int), 0);

	glVertexAttribDivisor(0, 0); // vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 0); // corner nr : always reuse the same 4 values -> 0
	glVertexAttribDivisor(2, 0); // texcoords : always reuse the same 4 texcoords -> 0
	glVertexAttribDivisor(3, 1); // position : one per quad (its center) -> 1

	return new Cache(vao, m_positionBuffer, face, xOffset, yOffset, zOffset);
}

VoxelCache::~VoxelCache()
{
	delete[] m_points;
}
