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

VoxelRenderer::VoxelRenderer()
	: m_begunRender(false)
	, m_begunCache(false)
	, m_blendEnabled(false)
{
	m_shaderId = ShaderManager::createShaderProgram("Assets/Shaders/voxelshaderinstanced.vert", NULL, "Assets/Shaders/voxelshaderinstanced.frag");
	
	glBindVertexArray(0);
	glUseProgram(m_shaderId);

	m_mvpUniformLoc = glGetUniformLocation(m_shaderId, MVP_UNIFORM_NAME);
	m_normalUniformLoc = glGetUniformLocation(m_shaderId, MVP_UNIFORM_NAME);

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

void VoxelRenderer::beginRender(const TextureArray* tileSet)
{
	assert(!m_begunRender && "Call end() before begin()");
	m_begunRender = true;
	tileSet->bind();
	glUseProgram(m_shaderId);
}

void VoxelRenderer::renderCache(Cache* const cache, const Camera& camera)
{
	assert(m_begunRender);

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix[3][0] = cache->m_xOffset * 0.5f; // TODO: figure out why / 2
	modelMatrix[3][1] = cache->m_yOffset * 0.5f;
	modelMatrix[3][2] = cache->m_zOffset * 0.5f;
	glm::mat4& mvpMatrix = camera.m_combinedMatrix * modelMatrix;
	setMVPUniform(mvpMatrix);

	for (int i = 0; i < 6; ++i)
	{
		switch (i)        //TODO: verify if normals are correct
		{
		case TOP:	setNormalUniform(glm::vec3(0.0, 1.0, 0.0));		break;
		case BOTTOM:setNormalUniform(glm::vec3(0.0, -1.0, 0.0));	break;
		case LEFT:	setNormalUniform(glm::vec3(-1.0, 0.0, 0.0));	break;
		case RIGHT: setNormalUniform(glm::vec3(1.0, 0.0, 0.0));		break;
		case FRONT: setNormalUniform(glm::vec3(0.0, 0.0, 1.0));		break;
		case BACK:	setNormalUniform(glm::vec3(0.0, 0.0, -1.0));	break;
		}

		// if there are no faces to render, skip
		PerFaceBufferData& faceBufferData = cache->m_faceBuffers[i];
		if (faceBufferData.m_numFaces == 0)
			continue;

		glBindVertexArray(faceBufferData.m_vao);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, faceBufferData.m_numFaces);
	}
}

void VoxelRenderer::endRender()
{
	assert(m_begunRender && "Call begin() before end()");
	m_begunRender = false;
	glUseProgram(0);
}

VoxelRenderer::Cache* const VoxelRenderer::createCache(float xOffset, float yOffset, float zOffset)
{
	assert(!m_begunRender && "Cannot create a new cache in between begin() and end()");

	Cache* cache = new Cache(xOffset, yOffset, zOffset);

	// for every face
	for (int i = 0; i < 6; ++i)
	{
		PerFaceBufferData& faceBufferData = cache->m_faceBuffers[i];
		glGenVertexArrays(1, &faceBufferData.m_vao);
		glBindVertexArray(faceBufferData.m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_faceVertexBuffer[i]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		//create position buffer unique to cache
		glGenBuffers(1, &faceBufferData.m_positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, faceBufferData.m_positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, MAX_FACES_PER_CACHE * sizeof(int), NULL, GL_STREAM_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(int), 0);

		//create color buffer unique to cache
		glGenBuffers(1, &faceBufferData.m_colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, faceBufferData.m_colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, MAX_FACES_PER_CACHE * sizeof(int) * 4, NULL, GL_STREAM_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4, NULL);

		glVertexAttribDivisor(0, 0); // vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 0); // texcoords : always reuse the same 4 texcoords -> 0
		glVertexAttribDivisor(2, 1); // position : one per quad (its center) -> 1
	}

	return cache;
}

VoxelRenderer::~VoxelRenderer()
{
	glDeleteShader(m_shaderId);
	glDeleteBuffers(6, &m_faceVertexBuffer[0]);
	glDeleteBuffers(1, &m_texCoordBuffer);
}

void VoxelRenderer::beginCache(VoxelRenderer::Cache* const cache)
{
	assert(!m_begunCache && "A cache has already begun");
	m_begunCache = true;

	// Clear all the counters
	for (int i = 0; i < 6; ++i) {
		cache->m_faceBuffers[i].m_numFaces = 0;
		m_data[i].m_colorIdx = 0;
		m_data[i].m_pointIdx = 0;
	}

	cache->m_data = m_data;
}

void VoxelRenderer::Cache::addFace(Face face, int x, int y, int z, int textureID, Color8888 color1, Color8888 color2, Color8888 color3, Color8888 color4)
{
	assert(m_data && "Cache has not yet begun");

	PerFaceVertexData& data = m_data[face];
	FacePointData pointData(x, y, z, textureID);
	data.m_points[data.m_pointIdx++] = pointData;
	data.m_colorBits[data.m_colorIdx++] = color1;
	data.m_colorBits[data.m_colorIdx++] = color2;
	data.m_colorBits[data.m_colorIdx++] = color3;
	data.m_colorBits[data.m_colorIdx++] = color4;

	m_faceBuffers[face].m_numFaces++;
}

void VoxelRenderer::endCache(VoxelRenderer::Cache* const cache)
{
	assert(m_begunCache && m_data && "Cache has not yet begun");
	m_begunCache = false;

	for (int i = 0; i < 6; ++i) 
	{
		PerFaceVertexData& faceVertexData = cache->m_data[i];
		PerFaceBufferData& faceBufferData = cache->m_faceBuffers[i];

		//update position data
		glBindVertexArray(faceBufferData.m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, faceBufferData.m_positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, MAX_FACES_PER_CACHE * sizeof(int), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, faceVertexData.m_pointIdx * sizeof(int), &faceVertexData.m_points);
		//update color data
		glBindBuffer(GL_ARRAY_BUFFER, faceBufferData.m_colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, MAX_FACES_PER_CACHE * sizeof(int) * 4, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, faceVertexData.m_colorIdx * sizeof(int), &faceVertexData.m_colorBits);
	}

	cache->m_data = NULL;
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

void VoxelRenderer::deleteCache(Cache* const cache)
{
	for (int i = 0; i < 6; ++i)
	{
		PerFaceBufferData& faceBufferData = cache->m_faceBuffers[i];

		glDeleteBuffers(1, &faceBufferData.m_colorBuffer);
		glDeleteBuffers(1, &faceBufferData.m_positionBuffer);
		glDeleteVertexArrays(1, &faceBufferData.m_vao);
	}

	delete cache;
}