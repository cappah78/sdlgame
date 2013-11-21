#include "VoxelCache.h"

#include "Camera.h"
#include "Material.h"
#include <glm/gtx/transform.hpp>

#include "ShaderManager.h"

const int WIDTH_BITS = 4;
const int HEIGHT_BITS = 4;

const char* CACHE_VERTEX_TRANSFORM_UNIFORM_NAME = "VertexTransform";
const int CACHE_VERTEX_TRANSFORM_BINDING_POINT = 0;

VoxelCache::VoxelCache(unsigned int sizeInFaces)
	: m_sizeInFaces(sizeInFaces)
	, m_drawing(false)
	, m_blendEnabled(false)
	, m_currentMaterial(NULL)
	, m_pointIdx(0)
{
	m_shaderId = ShaderManager::createShaderProgram("voxelshader.vert", "voxelshader.geom", "voxelshader.frag");
	
	glUseProgram(m_shaderId);

	m_points = new unsigned int[sizeInFaces];
	memset(m_points, 0, sizeInFaces * sizeof(int));

	glGenBuffers(1, &m_vertexTransformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_vertexTransformBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, CACHE_VERTEX_TRANSFORM_BINDING_POINT, m_vertexTransformBuffer);

	GLuint vertexTransformIdx = glGetUniformBlockIndex(m_shaderId, CACHE_VERTEX_TRANSFORM_UNIFORM_NAME);
	glUniformBlockBinding(m_shaderId, vertexTransformIdx, CACHE_VERTEX_TRANSFORM_BINDING_POINT);
}

VoxelCache::~VoxelCache()
{
	delete[] m_points;
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

	glBindVertexArray(cache->m_vao);
	m_currentCache = cache;

	glUseProgram(m_shaderId);
}

void VoxelCache::beginCache(Face face, float xOffset, float yOffset, float zOffset)
{
	assert(!m_drawing && "Call end() before begin()");
	m_drawing = true;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &m_pointBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_pointBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_sizeInFaces * sizeof(int), m_points, GL_STATIC_DRAW);
	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(int), 0);
	glEnableVertexAttribArray(0);

	m_currentCache = new Cache(vao, m_pointBuffer, face, xOffset, yOffset, zOffset);

	glUseProgram(m_shaderId);
}

void VoxelCache::renderCache(Cache* const cache, const Camera& camera)
{
	if (cache->m_amount == 0)
		return;

	glUseProgram(m_shaderId);
	glBindVertexArray(cache->m_vao);

	setUniforms(camera, cache->m_face, cache->m_xOffset, cache->m_yOffset, cache->m_zOffset);

	glDrawArrays(GL_POINTS, 0, cache->m_amount);

	glUseProgram(0);
}

void VoxelCache::deleteCache(Cache* const cache)
{
	glDeleteBuffers(1, &cache->m_pointBuffer);
	glDeleteVertexArrays(1, &cache->m_vao);
	delete cache;
}

VoxelCache::Cache* const VoxelCache::createCache(Face face, float xOffset, float yOffset, float zOffset)
{
	assert(!m_drawing && "Cannot create a new cache in between begin() and end()");
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &m_pointBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_pointBuffer);
	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(int), 0);
	glEnableVertexAttribArray(0);

	return new Cache(vao, m_pointBuffer, face, xOffset, yOffset, zOffset);
}

void VoxelCache::setUniforms(const Camera& camera, Face face, float xOffset, float yOffset, float zOffset)
{
	//copy camera matrix
	m_vertexTransform.V1MVPMatrix = camera.m_combinedMatrix;
	m_vertexTransform.V2MVPMatrix = camera.m_combinedMatrix;
	m_vertexTransform.V3MVPMatrix = camera.m_combinedMatrix;
	m_vertexTransform.V4MVPMatrix = camera.m_combinedMatrix;
	glm::vec3 v1Offset(xOffset, yOffset, zOffset);
	glm::vec3 v2Offset(xOffset, yOffset, zOffset);
	glm::vec3 v3Offset(xOffset, yOffset, zOffset);
	glm::vec3 v4Offset(xOffset, yOffset, zOffset);

	switch (face)
	{
	case TOP:
		v1Offset += glm::vec3(1.0f, 1.0f, 0.0f);
		v2Offset += glm::vec3(0.0f, 1.0f, 0.0f);
		v3Offset += glm::vec3(1.0f, 1.0f, 1.0f);
		v4Offset += glm::vec3(0.0f, 1.0f, 1.0f);
		break;
	case BOTTOM:
		v1Offset += glm::vec3(1.0f, 0.0f, 1.0f);
		v2Offset += glm::vec3(0.0f, 0.0f, 1.0f);
		v3Offset += glm::vec3(1.0f, 0.0f, 0.0f);
		v4Offset += glm::vec3(0.0f, 0.0f, 0.0f);
		break;
	case LEFT:
		v1Offset += glm::vec3(0.0f, 1.0f, 0.0f);
		v2Offset += glm::vec3(1.0f, 1.0f, 0.0f);
		v3Offset += glm::vec3(0.0f, 0.0f, 0.0f);
		v4Offset += glm::vec3(1.0f, 0.0f, 0.0f);
		break;
	case RIGHT:
		v1Offset += glm::vec3(1.0f, 1.0f, 1.0f);
		v2Offset += glm::vec3(0.0f, 1.0f, 1.0f);
		v3Offset += glm::vec3(1.0f, 0.0f, 1.0f);
		v4Offset += glm::vec3(0.0f, 0.0f, 1.0f);
		break;
	case FRONT:
		v1Offset += glm::vec3(1.0f, 1.0f, 0.0f);
		v2Offset += glm::vec3(1.0f, 1.0f, 1.0f);
		v3Offset += glm::vec3(1.0f, 0.0f, 0.0f);
		v4Offset += glm::vec3(1.0f, 0.0f, 1.0f);
		break;
	case BACK:
		v1Offset += glm::vec3(0.0f, 1.0f, 1.0f);
		v2Offset += glm::vec3(0.0f, 1.0f, 0.0f);
		v3Offset += glm::vec3(0.0f, 0.0f, 1.0f);
		v4Offset += glm::vec3(0.0f, 0.0f, 0.0f);
		break;
	}

	m_vertexTransform.V1MVPMatrix *= glm::translate(v1Offset);
	m_vertexTransform.V2MVPMatrix *= glm::translate(v2Offset);
	m_vertexTransform.V3MVPMatrix *= glm::translate(v3Offset);
	m_vertexTransform.V4MVPMatrix *= glm::translate(v4Offset);

	glBindBuffer(GL_UNIFORM_BUFFER, m_vertexTransformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(VertexTransform), &m_vertexTransform, GL_STATIC_DRAW);
}

VoxelCache::Cache* const VoxelCache::endCache()
{
	assert(m_drawing && "Call begin() before end()");
	m_drawing = false;

	glBindBuffer(GL_ARRAY_BUFFER, m_pointBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_pointIdx * sizeof(int), m_points);

	glUseProgram(0);
	m_pointIdx = 0;

	Cache* const currentCache = m_currentCache;
	m_currentCache = NULL;

	return currentCache;
}

void VoxelCache::addFace(int x, int y, int z, const Material& material)
{
	assert(m_drawing && "Call beginCache() before addFace()");
	const Material* mat = &material;
	if (m_currentMaterial == NULL)
		swapMaterial(mat);

	assert(m_currentCache->m_amount < m_sizeInFaces && "Amount of draw calls exceeded size");
	m_currentCache->m_amount++;

	unsigned int index = x | (y | z << HEIGHT_BITS) << WIDTH_BITS;
	m_points[m_pointIdx++] = index;
}

void VoxelCache::swapMaterial(const Material* const material)
{
	m_currentMaterial = material;
	const Texture& t = m_currentMaterial->getDiffuse()->m_texture;

	if (t.getNumComponents() == 4) //rgba
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_blendEnabled = true;
	}
	else if (m_blendEnabled)
	{
		glDisable(GL_BLEND);
		m_blendEnabled = false;
	}

	t.bind();
}