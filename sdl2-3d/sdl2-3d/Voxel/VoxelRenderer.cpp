#include "VoxelRenderer.h"

#include "../Game.h"
#include "../Engine/Graphics/Camera.h"

#include "../Engine/Graphics/Model/IStateBuffer.h"
#include "../Engine/Graphics/Model/IIndiceBuffer.h"
#include "../Engine/Graphics/Model/IVertexBuffer.h"

#include <glm/gtx/transform.hpp>

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
	: m_begunChunk(false)
	, m_blendEnabled(false)
{
	m_pointData.reserve(MAX_FACES_PER_CHUNK * 4);
	m_indiceData.reserve(MAX_FACES_PER_CHUNK * 6);
	m_texcoordData.reserve(MAX_FACES_PER_CHUNK * 4);

	m_texcoordBuffer = Game::graphics.getGraphicsProvider().createVertexBuffer();

	for (int i = 0; i < MAX_FACES_PER_CHUNK; ++i)
	{
		m_texcoordData.push_back(glm::vec2(1.0f, 0.0f));
		m_texcoordData.push_back(glm::vec2(0.0f, 0.0f));
		m_texcoordData.push_back(glm::vec2(1.0f, 1.0f));
		m_texcoordData.push_back(glm::vec2(0.0f, 1.0f));
	}
	m_texcoordBuffer->update(&m_texcoordData[0], m_texcoordData.size() * sizeof(m_texcoordData[0]));
	m_texcoordData.clear();
}

VoxelRenderer::~VoxelRenderer()
{
	for (Chunk* chunk : m_chunkPool)
		delete chunk;
}

void VoxelRenderer::renderChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk, GLenum mode)
{
	if (chunk->m_numFaces == 0)
		return;

	chunk->m_state->enable();
	glDrawElements(mode, chunk->m_numFaces * 6, GL_UNSIGNED_SHORT, 0);
	chunk->m_state->disable();
}

const std::shared_ptr<VoxelRenderer::Chunk> VoxelRenderer::createChunk(float xOffset, float yOffset, float zOffset)
{
	if (m_chunkPool.size() > 0)
	{
		std::shared_ptr<VoxelRenderer::Chunk> chunk(m_chunkPool.back(), &returnChunk);
		chunk->m_renderOffset.x = xOffset;
		chunk->m_renderOffset.y = yOffset;
		chunk->m_renderOffset.z = zOffset;
		m_chunkPool.pop_back();
		return chunk;
	}
	std::shared_ptr<VoxelRenderer::Chunk> chunk(new Chunk(xOffset, yOffset, zOffset, *this), &returnChunk);
	return chunk;
}

static const unsigned int MAX_CHUNKS_IN_POOL = 100;
void VoxelRenderer::returnChunk(Chunk* chunk)
{
	if (chunk->m_renderer.m_chunkPool.size() < MAX_CHUNKS_IN_POOL)
		chunk->m_renderer.m_chunkPool.push_back(chunk);
	else
		delete chunk;
}

void VoxelRenderer::beginChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk)
{
	assert(!chunk->m_begun && "This chunk has already begun");
	assert(!m_begunChunk && "A chunk has already begun");
	m_begunChunk = true;

	chunk->m_numFaces = 0;
	chunk->m_begun = true;
}

void VoxelRenderer::Chunk::addFace(Face face, int x, int y, int z, int textureID, unsigned char vertex1AO, unsigned char vertex2AO, unsigned char vertex3AO, unsigned char vertex4AO, bool flipQuad)
{
	assert(m_begun && "Chunk has not yet begun");

	std::vector<unsigned short>& indiceData = m_renderer.m_indiceData;
	std::vector<VoxelVertex>& pointData = m_renderer.m_pointData;

	m_numFaces++;
	unsigned int indiceIdx = indiceData.size();
	unsigned short j = (indiceIdx / 6) * 4;

	if (flipQuad)
	{
		indiceData.push_back(j + 1);
		indiceData.push_back(j + 3);
		indiceData.push_back(j + 0);
		indiceData.push_back(j + 2);
		indiceData.push_back(j + 0);
		indiceData.push_back(j + 3);
	}
	else
	{
		indiceData.push_back(j + 1);
		indiceData.push_back(j + 3);
		indiceData.push_back(j + 2);
		indiceData.push_back(j + 0);
		indiceData.push_back(j + 1);
		indiceData.push_back(j + 2);
	}

	unsigned int aoValues[] = { vertex1AO, vertex2AO, vertex3AO, vertex4AO };
	for (int i = 0; i < 4; ++i)
	{
		int xOffset = g_faceVertexOffsetsInt[face][i][0];
		int yOffset = g_faceVertexOffsetsInt[face][i][1];
		int zOffset = g_faceVertexOffsetsInt[face][i][2];

		VoxelVertex vertex(x + xOffset, y + yOffset, z + zOffset, textureID, aoValues[i]);
		pointData.push_back(vertex);
	}
}

void VoxelRenderer::endChunk(const std::shared_ptr<VoxelRenderer::Chunk>& chunk)
{
	assert(m_begunChunk && chunk->m_begun && "Chunk has not yet begun");
	m_begunChunk = false;
	chunk->m_begun = false;

	if (chunk->m_numFaces == 0)
		return;

	chunk->m_state->enable();
	chunk->m_indiceBuffer->update(&m_indiceData[0], m_indiceData.size() * sizeof(m_indiceData[0]));
	chunk->m_pointBuffer->update(&m_pointData[0], m_pointData.size() * sizeof(m_pointData[0]));
	chunk->m_state->disable();

	m_indiceData.clear();
	m_pointData.clear();
}

VoxelRenderer::Chunk::Chunk(float xOffset, float yOffset, float zOffset, VoxelRenderer& renderer)
	: m_renderOffset(xOffset, yOffset, zOffset)
	, m_numFaces(0)
	, m_begun(false)
	, m_renderer(renderer)
{
	IGraphicsProvider& provider = Game::graphics.getGraphicsProvider();

	m_state = provider.createStateBuffer();
	m_indiceBuffer = provider.createIndiceBuffer(IIndiceBufferParameters(IIndiceBufferParameters::Format::UNSIGNED_SHORT));
	m_pointBuffer = provider.createVertexBuffer();

	VertexAttribute pointAttrib = { POSITION_LOC, "POINTDATA", VertexAttribute::Format::UNSIGNED_INT, 1 };
	VertexAttributes pointAttributes(&pointAttrib, 1);

	VertexAttribute texcoordAttrib( TEXCOORD_LOC, "TEXCOORD", VertexAttribute::Format::FLOAT, 2);
	VertexAttributes texcoordAttributes(&texcoordAttrib, 1);

	m_state->enable();
	m_state->addVertexBuffer(m_pointBuffer);
	m_state->addVertexBuffer(renderer.m_texcoordBuffer);
	m_state->setIndiceBuffer(m_indiceBuffer);
	m_pointBuffer->setVertexAttributeParameters(pointAttributes);
	renderer.m_texcoordBuffer->setVertexAttributeParameters(texcoordAttributes);
	m_state->disable();
}

VoxelRenderer::Chunk::~Chunk()
{

}