#include "ChunkRenderer.h"

#include "../Engine/Graphics/Camera.h"

#include <glm\glm.hpp>

ChunkRenderer::ChunkRenderer()
{

}

ChunkRenderer::~ChunkRenderer()
{

}

ChunkRenderData* const ChunkRenderer::createChunkRenderData(float xOffset, float yOffset, float zOffset)
{
	ChunkRenderData* const data = new ChunkRenderData(xOffset, yOffset, zOffset);
	for (unsigned int i = 0; i < 6; ++i)
		data->m_caches[i] = m_voxelCache.createCache((VoxelCache::Face)(VoxelCache::TOP + i));

	return data;
}

void ChunkRenderer::deleteChunkRenderData(ChunkRenderData* const chunkRenderData)
{
	for (unsigned int i = 0; i < 6; ++i)
		m_voxelCache.deleteCache(chunkRenderData->m_caches[i]);
	delete chunkRenderData;
}

void ChunkRenderer::beginAdd(ChunkRenderData* const chunkRenderData)
{
	assert(!chunkRenderData->m_begun);
	chunkRenderData->m_begun = true;
	for (unsigned int i = 0; i < 6; ++i)
		m_voxelCache.beginCache(chunkRenderData->m_caches[i]);
}

void ChunkRenderData::addFace(VoxelCache::Face face, int x, int y, int z, int textureIdx, int color1, int color2, int color3, int color4)
{
	assert(m_begun);
	m_caches[face]->addFace(x, y, z, textureIdx, color1, color2, color3, color4);
}

void ChunkRenderer::endAdd(ChunkRenderData* const chunkRenderData)
{
	assert(chunkRenderData->m_begun);
	chunkRenderData->m_begun = false;
	for (unsigned int i = 0; i < 6; ++i)
		m_voxelCache.endCache(chunkRenderData->m_caches[i]);
}

void ChunkRenderer::renderChunk(const ChunkRenderData* const chunkRenderData, const Camera& camera)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix[3][0] = chunkRenderData->m_xOffset * 0.5f;        //TODO: figure out why / 2
	modelMatrix[3][1] = chunkRenderData->m_yOffset * 0.5f;
	modelMatrix[3][2] = chunkRenderData->m_zOffset * 0.5f;
	glm::mat4& mvpMatrix = camera.m_combinedMatrix * modelMatrix;
	m_voxelCache.setMVPUniform(mvpMatrix);

	for (int face = 0; face < 6; ++face)
	{
		switch (face)        //TODO: verify if normals are correct
		{
		case VoxelCache::TOP:	m_voxelCache.setNormalUniform(glm::vec3(0.0, 1.0, 0.0));	break;
		case VoxelCache::BOTTOM:m_voxelCache.setNormalUniform(glm::vec3(0.0, -1.0, 0.0));	break;
		case VoxelCache::LEFT:	m_voxelCache.setNormalUniform(glm::vec3(-1.0, 0.0, 0.0));	break;
		case VoxelCache::RIGHT: m_voxelCache.setNormalUniform(glm::vec3(1.0, 0.0, 0.0));	break;
		case VoxelCache::FRONT: m_voxelCache.setNormalUniform(glm::vec3(0.0, 0.0, 1.0));	break;
		case VoxelCache::BACK:	m_voxelCache.setNormalUniform(glm::vec3(0.0, 0.0, -1.0));	break;
		}
		m_voxelCache.renderCache(chunkRenderData->m_caches[face], camera);
	}
}

void ChunkRenderer::beginRender(const TextureArray* tileSet)
{
	m_voxelCache.beginRender(tileSet);
}

void ChunkRenderer::endRender()
{
	m_voxelCache.finishRender();
}