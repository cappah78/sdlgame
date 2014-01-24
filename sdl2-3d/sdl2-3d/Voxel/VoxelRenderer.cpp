#include "VoxelRenderer.h"

#include "..\Engine\Graphics\Camera.h"
#include <glm/gtx/transform.hpp>
#include "../Engine/Utils/CheckGLError.h"


const char* VERT_SHADER_PATH = "Assets/Shaders/voxelshader.vert";
const char* FRAG_SHADER_PATH = "Assets/Shaders/voxelshader.frag";

// [face][corner][vertices]
static const int g_faceVertexOffsetsInt[6][4][3] = {
	{ //top
		{ 1, 1, 0 }, { 0, 1, 0 },
		{ 1, 1, 1 }, { 0, 1, 1 }},
	{ //bottom
		{ 1, 0, 1 }, { 0, 0, 1 },
		{ 1, 0, 0 }, { 0, 0, 0 }},
	{ //left
		{ 0, 1, 0 }, { 1, 1, 0 },
		{ 0, 0, 0 }, { 1, 0, 0 }},
	{ //right
		{ 1, 1, 1 }, { 0, 1, 1 },
		{ 1, 0, 1 }, { 0, 0, 1 }},
	{ //front
		{ 1, 1, 0 }, { 1, 1, 1 },
		{ 1, 0, 0 }, { 1, 0, 1 }},
	{ //back
		{ 0, 1, 1 }, { 0, 1, 0 },
		{ 0, 0, 1 }, { 0, 0, 0 }
	}
};

VoxelRenderer::VoxelRenderer()
	: m_begunRender(false)
	, m_begunChunk(false)
	, m_blendEnabled(false)
	, m_texcoordBuffer(MAX_FACES_PER_CHUNK * 4)
	, m_vertexIdBuffer(MAX_FACES_PER_CHUNK * 4)
{
	CHECK_GL_ERROR();
	m_pointData.resize(MAX_FACES_PER_CHUNK * 4);
	m_colorData.resize(MAX_FACES_PER_CHUNK * 4);
	m_indiceData.resize(MAX_FACES_PER_CHUNK * 6);
	CHECK_GL_ERROR();
	glBindVertexArray(0);

	unsigned short vertexId = 0;
	for (int i = 0; i < MAX_FACES_PER_CHUNK; ++i)
	{
		m_texcoordBuffer.add(glm::vec2(1.0f, 0.0f));
		m_texcoordBuffer.add(glm::vec2(0.0f, 0.0f));
		m_texcoordBuffer.add(glm::vec2(1.0f, 1.0f));
		m_texcoordBuffer.add(glm::vec2(0.0f, 1.0f));
		m_vertexIdBuffer.add(vertexId++);
		m_vertexIdBuffer.add(vertexId++);
		m_vertexIdBuffer.add(vertexId++);
		m_vertexIdBuffer.add(vertexId++);
	}
	m_texcoordBuffer.update();
	m_vertexIdBuffer.update();

	CHECK_GL_ERROR();
}

VoxelRenderer::~VoxelRenderer()
{
	for (Chunk* chunk : m_chunkPool)
	{
		delete chunk;
	}
	CHECK_GL_ERROR();
}

void VoxelRenderer::renderChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk, GLenum mode)
{
	if (chunk->m_numFaces == 0)
		return;

	glBindVertexArray(chunk->m_vao);
	glDrawElements(mode, chunk->m_numFaces * 6, GL_UNSIGNED_SHORT, 0);
}

const std::shared_ptr<VoxelRenderer::Chunk> VoxelRenderer::createChunk(float xOffset, float yOffset, float zOffset)
{
	if (m_chunkPool.size() > 0)
	{
		std::shared_ptr<VoxelRenderer::Chunk> chunk(m_chunkPool.back(), &returnChunk);
		chunk->m_renderOffset.x = xOffset;
		chunk->m_renderOffset.y = yOffset;
		chunk->m_renderOffset.z = zOffset;
		memset(&m_colorData[0], 0, sizeof(Color8888) * m_colorData.size());
		chunk->m_colorBuffer.update();

		m_chunkPool.pop_back();
		return chunk;
	}

	Chunk* newChunk = new Chunk(xOffset, yOffset, zOffset, m_colorData, m_pointData, m_indiceData, *this);
	std::shared_ptr<VoxelRenderer::Chunk> chunk(newChunk, &returnChunk);
	m_texcoordBuffer.bind();
	memset(&m_colorData[0], 0, sizeof(Color8888) * m_colorData.size());
	newChunk->m_colorBuffer.update();

	CHECK_GL_ERROR();

	return chunk;
}

void VoxelRenderer::returnChunk(Chunk* chunk)
{
	chunk->m_renderer.m_chunkPool.push_back(chunk);
}

void VoxelRenderer::beginChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk)
{
	assert(!chunk->m_begun && "This chunk has already begun");
	assert(!m_begunChunk && "A chunk has already begun");
	m_begunChunk = true;

	chunk->m_numFaces = 0;

	chunk->m_indiceBuffer.reset();
	chunk->m_pointBuffer.reset();
	chunk->m_colorBuffer.reset();

	chunk->m_begun = true;
}

void VoxelRenderer::Chunk::addFace(Face face, int x, int y, int z, int textureID, Color8888 color1, Color8888 color2, Color8888 color3, Color8888 color4, bool flipQuad)
{
	assert(m_begun && "Chunk has not yet begun");

	m_numFaces++;
	unsigned int indiceIdx = m_indiceBuffer.getSizeInElements();
	unsigned short j = (indiceIdx / 6) * 4;

	if (flipQuad)
	{
		m_indiceBuffer.add(j + 1);
		m_indiceBuffer.add(j + 3);
		m_indiceBuffer.add(j + 0);
		m_indiceBuffer.add(j + 2);
		m_indiceBuffer.add(j + 0);
		m_indiceBuffer.add(j + 3);
	}
	else
	{
		m_indiceBuffer.add(j + 1);
		m_indiceBuffer.add(j + 3);
		m_indiceBuffer.add(j + 2);
		m_indiceBuffer.add(j + 0);
		m_indiceBuffer.add(j + 1);
		m_indiceBuffer.add(j + 2);
	}

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
}

void VoxelRenderer::endChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk)
{
	assert(m_begunChunk && chunk->m_begun && "Chunk has not yet begun");
	m_begunChunk = false;

	chunk->m_indiceBuffer.update();
	chunk->m_pointBuffer.update();
	chunk->m_colorBuffer.update();

	chunk->m_begun = false;

	CHECK_GL_ERROR();
}

VoxelRenderer::Chunk::Chunk(float xOffset, float yOffset, float zOffset
	, std::vector<Color8888>& colorData
	, std::vector<VoxelVertex>& pointData
	, std::vector<unsigned short>& indiceData
	, VoxelRenderer& renderer)
	: m_colorBuffer(colorData.size(), GL_ARRAY_BUFFER, GL_STREAM_DRAW, &colorData[0])
	, m_pointBuffer(pointData.size(), GL_ARRAY_BUFFER, GL_STREAM_DRAW, &pointData[0])
	, m_indiceBuffer(indiceData.size(), GL_ELEMENT_ARRAY_BUFFER, GL_STREAM_DRAW, &indiceData[0])
	, m_renderOffset(xOffset, yOffset, zOffset)
	, m_numFaces(0)
	, m_begun(false)
	, m_renderer(renderer)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	m_pointBuffer.setAttribPointer(POSITION_LOC, GL_UNSIGNED_INT, 1, GL_FALSE, GL_TRUE);
	m_colorBuffer.setAttribPointer(COLOR_LOC, GL_UNSIGNED_BYTE, 4, GL_TRUE);
	m_renderer.m_texcoordBuffer.setAttribPointer(TEXCOORD_LOC, GL_FLOAT, 2);
	m_renderer.m_vertexIdBuffer.setAttribPointer(3, GL_UNSIGNED_SHORT, 1, GL_FALSE, GL_TRUE);

	CHECK_GL_ERROR();
}

VoxelRenderer::Chunk::~Chunk()
{
	glDeleteVertexArrays(1, &m_vao);
	CHECK_GL_ERROR();
}