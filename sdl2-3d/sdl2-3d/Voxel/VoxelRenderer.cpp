#include "VoxelRenderer.h"

#include "..\Engine\Graphics\Camera.h"
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
	: m_begunRender(false)
	, m_begunChunk(false)
	, m_blendEnabled(false)
	, m_texcoordBuffer(MAX_FACES_PER_CACHE * 4)
	, m_shader(VERT_SHADER_PATH, NULL, FRAG_SHADER_PATH)
{
	m_pointData.resize(MAX_FACES_PER_CACHE * 4);
	m_colorData.resize(MAX_FACES_PER_CACHE * 4);
	m_indiceData.resize(MAX_FACES_PER_CACHE * 6);
	
	glBindVertexArray(0);

	for (int i = 0; i < MAX_FACES_PER_CACHE; ++i)
	{
		m_texcoordBuffer.add(glm::vec2(1.0f, 0.0f));
		m_texcoordBuffer.add(glm::vec2(0.0f, 0.0f));
		m_texcoordBuffer.add(glm::vec2(1.0f, 1.0f));
		m_texcoordBuffer.add(glm::vec2(0.0f, 1.0f));
	}
	m_texcoordBuffer.update();
}

VoxelRenderer::~VoxelRenderer()
{

}

void VoxelRenderer::beginRender(const TextureArray* tileSet)
{
	assert(!m_begunRender && "Call endRender() before beginRender()");
	m_begunRender = true;
	tileSet->bind();
	m_shader.begin();
}

void VoxelRenderer::renderChunk(const std::shared_ptr<VoxelRenderer::Chunk> chunk, const Camera& camera)
{
	assert(m_begunRender);

	m_shader.setUniform3f("u_fogColor", glm::vec3(0.4f, 0.7f, 1.0f));	//same as clearcolor
	m_shader.setUniform1f("u_fogEnd", camera.m_far * 0.9f);
	m_shader.setUniform1f("u_fogStart", camera.m_far * 0.6f);
	m_shader.setUniform3f("u_chunkOffset", chunk->m_renderOffset);
	m_shader.setUniform3f("u_camPos", camera.m_position);
	m_shader.setUniformMatrix4f("u_mvp", camera.m_combinedMatrix);

	glBindVertexArray(chunk->m_vao);
	chunk->m_indiceBuffer.bind();
	glDrawElements(GL_TRIANGLES, chunk->m_numFaces * 6, GL_UNSIGNED_SHORT, 0);
}

void VoxelRenderer::endRender()
{
	assert(m_begunRender && "Call beginRender() before endRender()");
	m_begunRender = false;
	m_shader.end();
}

const std::shared_ptr<VoxelRenderer::Chunk> VoxelRenderer::createChunk(float xOffset, float yOffset, float zOffset, const glm::vec3* const bounds)
{
	assert(!m_begunRender && "Cannot create a new chunk in between beginRender() and endRender()");

	std::shared_ptr<VoxelRenderer::Chunk> chunk(new Chunk(xOffset, yOffset, zOffset, bounds, m_colorData, m_pointData, m_indiceData));
	glGenVertexArrays(1, &chunk->m_vao);
	glBindVertexArray(chunk->m_vao);

	chunk->m_pointBuffer.setAttribPointer(POSITION_LOC, GL_UNSIGNED_INT, 1, GL_FALSE, GL_TRUE);
	chunk->m_colorBuffer.setAttribPointer(COLOR_LOC, GL_UNSIGNED_BYTE, 4, GL_TRUE);
	m_texcoordBuffer.setAttribPointer(TEXCOORD_LOC, GL_FLOAT, 2);

	return chunk;
}

void VoxelRenderer::beginChunk(const std::shared_ptr<VoxelRenderer::Chunk> chunk)
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

void VoxelRenderer::endChunk(const std::shared_ptr<VoxelRenderer::Chunk> chunk)
{
	assert(m_begunChunk && chunk->m_begun && "Chunk has not yet begun");
	m_begunChunk = false;

	chunk->m_indiceBuffer.update();
	chunk->m_pointBuffer.update();
	chunk->m_colorBuffer.update();

	chunk->m_begun = false;
}

VoxelRenderer::Chunk::~Chunk()
{
	glDeleteVertexArrays(1, &m_vao);
}