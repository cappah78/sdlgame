#include "VoxelCache.h"

#include "..\Engine\Graphics\Camera.h"
#include "..\Engine\Graphics\Material.h"
#include "..\Engine\Utils\ShaderManager.h"
#include <glm/gtx/transform.hpp>

const unsigned int SIZE_BITS = 4; //2^sizebits == chunksize or sqrt(chunkSize)
const unsigned int INDEX_BITS = 3 * SIZE_BITS;
const unsigned int TEXTURE_ID_BITS = 12;

const char* CACHE_VERTEX_TRANSFORM_UNIFORM_NAME = "VoxelTransform2";
const int CACHE_VERTEX_TRANSFORM_BINDING_POINT = 0;

const char* MVP_UNIFORM_NAME = "u_mvp";
const char* NORMAL_UNIFORM_NAME = "u_normal";

const unsigned int INDEX_MASK = 0x00000FFFu;
const unsigned int TEXTURE_ID_MASK = 0x00FFF000u;
const unsigned int LIGHT_LEVEL_MASK = 0xFF000000u;

static const GLfloat g_texCoordData[] = {
	1.0, 0.0,
	0.0, 0.0,
	1.0, 1.0,
	0.0, 1.0
};

static const GLfloat g_faceVertexData [6][12] = {
	{	//top
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f },
	{	//bottom
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f },
	{	//left
		0.0f, 1.0f, 0.0f,
		1.0, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f },
	{	//right
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f },
	{	//front
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f },
	{	//back
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f }
};

VoxelCache::VoxelCache()
	: m_drawing(false)
	, m_blendEnabled(false)
{
	m_shaderId = ShaderManager::createShaderProgram("Assets/Shaders/voxelshaderinstanced.vert", NULL, "Assets/Shaders/voxelshaderinstanced.frag");
	
	glBindVertexArray(0);
	glUseProgram(m_shaderId);

	m_mvpUniformLoc = glGetUniformLocation(m_shaderId, MVP_UNIFORM_NAME);
	m_normalUniformLoc = glGetUniformLocation(m_shaderId, MVP_UNIFORM_NAME);

	GLuint vertexTransformIdx = glGetUniformBlockIndex(m_shaderId, CACHE_VERTEX_TRANSFORM_UNIFORM_NAME);
	glUniformBlockBinding(m_shaderId, vertexTransformIdx, CACHE_VERTEX_TRANSFORM_BINDING_POINT);

	glGenBuffers(1, &m_texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_texCoordData), g_texCoordData, GL_STREAM_DRAW);

	for (int i = 0; i < 6; ++i)	//setup buffers for the 6 faces
	{
		glGenBuffers(1, &m_faceVertexBuffer[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_faceVertexBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_faceVertexData[i]), g_faceVertexData[i], GL_STREAM_DRAW);
	}
}

void VoxelCache::beginRender(const TextureArray* tileSet)
{
	assert(!m_drawing && "Call end() before begin()");
	m_drawing = true;
	tileSet->bind();
	glUseProgram(m_shaderId);
}

void VoxelCache::renderCache(Cache* const cache, const Camera& camera)
{
	assert(m_drawing);
	if (cache->m_numFaces == 0)
		return;

	glBindVertexArray(cache->m_vao); 
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, cache->m_numFaces);
}

void VoxelCache::finishRender()
{
	assert(m_drawing && "Call begin() before end()");
	m_drawing = false;
	glUseProgram(0);
}

VoxelCache::Cache* const VoxelCache::createCache(Face face)
{
	assert(!m_drawing && "Cannot create a new cache in between begin() and end()");

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_faceVertexBuffer[face]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	//create position buffer unique to cache
	GLuint positionBuffer;
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_FACES_PER_CACHE * sizeof(int), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(int), 0);

	//create color buffer unique to cache
	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_FACES_PER_CACHE * sizeof(int) * 4, NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(3);
	// only use rgb
	glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4, NULL);

	//TODO: color buffer and attrib

	glVertexAttribDivisor(0, 0); // vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 0); // texcoords : always reuse the same 4 texcoords -> 0
	glVertexAttribDivisor(2, 1); // position : one per quad (its center) -> 1

	return new Cache(vao, positionBuffer, colorBuffer, face);
}

VoxelCache::~VoxelCache()
{
	glDeleteShader(m_shaderId);
	glDeleteBuffers(6, &m_faceVertexBuffer[0]);
	glDeleteBuffers(1, &m_texCoordBuffer);
}

void VoxelCache::beginCache(VoxelCache::Cache* const cache)
{
	assert(!m_cacheData[cache->m_face].m_begun && "A cache with this face is already begun");
	assert(!cache->m_data && "Cache has already begun");

	cache->m_data = &m_cacheData[cache->m_face];
	m_cacheData[cache->m_face].m_begun = true;
}

void VoxelCache::Cache::addFace(int x, int y, int z, int textureID, Color8888 color1, Color8888 color2, Color8888 color3, Color8888 color4)
{
	assert(m_data && "Cache has not yet begun");

	m_numFaces++;
	// position+texId are being packed in one int here
	unsigned int index = x | (y | z << SIZE_BITS) << SIZE_BITS;
	unsigned int vertexData = index;
	vertexData |= textureID << INDEX_BITS;

	m_data->m_points[m_data->m_pointIdx++] = vertexData;

	m_data->m_colorBits[m_data->m_colorIdx++] = color1;
	m_data->m_colorBits[m_data->m_colorIdx++] = color2;
	m_data->m_colorBits[m_data->m_colorIdx++] = color3;
	m_data->m_colorBits[m_data->m_colorIdx++] = color4;
}

void VoxelCache::endCache(VoxelCache::Cache* const cache)
{
	assert(m_cacheData[cache->m_face].m_begun && "A cache with this face has not begun");
	assert(cache->m_data && "Cache has not yet begun");
	m_cacheData[cache->m_face].m_begun = false;

	//upload position data
	glBindVertexArray(cache->m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, cache->m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_FACES_PER_CACHE * sizeof(int), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, cache->m_data->m_pointIdx * sizeof(int), &cache->m_data->m_points);

	glBindBuffer(GL_ARRAY_BUFFER, cache->m_colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_FACES_PER_CACHE * sizeof(int) * 4, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, cache->m_data->m_colorIdx * sizeof(int), &cache->m_data->m_colorBits);

	//TODO: update color buffer

	m_cacheData[cache->m_face].m_pointIdx = 0;
	m_cacheData[cache->m_face].m_colorIdx = 0;

	cache->m_data = NULL;
}

void VoxelCache::setMVPUniform(glm::mat4 mvpMatrix)
{
	assert(m_drawing);
	glUniformMatrix4fv(m_mvpUniformLoc, 1, GL_FALSE, &mvpMatrix[0][0]);
}
void VoxelCache::setNormalUniform(glm::vec3 normal)
{
	assert(m_drawing);
	glUniform3fv(m_normalUniformLoc, 1, &normal[0]);
}

void VoxelCache::deleteCache(Cache* const cache)
{
	glDeleteBuffers(1, &cache->m_positionBuffer);
	glDeleteVertexArrays(1, &cache->m_vao);
	delete cache;
}