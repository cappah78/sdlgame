#include "ChunkRenderer.h"

ChunkRenderer::ChunkRenderer(float offsetX, float offsetY, float offsetZ)
	: m_offsetX(offsetX)
	, m_offsetY(offsetY)
	, m_offsetZ(offsetZ)
	, m_voxelCache()
{
	m_topFaceCache = m_voxelCache.createCache(VoxelCache::TOP, CHUNK_SIZE_CUBED, offsetX, offsetY, offsetZ);
	m_bottomFaceCache = m_voxelCache.createCache(VoxelCache::BOTTOM, CHUNK_SIZE_CUBED, offsetX, offsetY, offsetZ);
	m_leftFaceCache = m_voxelCache.createCache(VoxelCache::LEFT, CHUNK_SIZE_CUBED, offsetX, offsetY, offsetZ);
	m_rightFaceCache = m_voxelCache.createCache(VoxelCache::RIGHT, CHUNK_SIZE_CUBED, offsetX, offsetY, offsetZ);
	m_frontFaceCache = m_voxelCache.createCache(VoxelCache::FRONT, CHUNK_SIZE_CUBED, offsetX, offsetY, offsetZ);
	m_backFaceCache = m_voxelCache.createCache(VoxelCache::BACK, CHUNK_SIZE_CUBED, offsetX, offsetY, offsetZ);
}

ChunkRenderer::~ChunkRenderer()
{

}

void ChunkRenderer::begin()
{

}

void ChunkRenderer::renderFace(VoxelCache::Face face, int x, int y, int z, int textureIdx, float color1, float color2, float color3, float color4)
{
	switch (face)
	{
	case VoxelCache::TOP:
		//m_topFaceCache->addFace(x, y, z, textureIdx, color1, color2, color3, color4);
		break;
	case VoxelCache::BOTTOM:
		break;
	case VoxelCache::LEFT:
		break;
	case VoxelCache::RIGHT:
		break;
	case VoxelCache::FRONT:
		break;
	case VoxelCache::BACK:
		break;
	}
}

void ChunkRenderer::end()
{

}