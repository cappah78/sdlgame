#include "ChunkRenderer.h"

ChunkRenderer::ChunkRenderer()
{
	for (unsigned int i = 0; i < 6; ++i)
		m_caches[i] = m_voxelCache.createCache((VoxelCache::Face)(VoxelCache::TOP + i));
}

ChunkRenderer::~ChunkRenderer()
{
	for (unsigned int i = 0; i < 6; ++i)
		m_voxelCache.deleteCache(m_caches[i]);
}

void ChunkRenderer::begin(const TextureArray* tileSet, float xOffset, float yOffset, float zOffset, const Camera* camera)
{
	m_tileSet = tileSet;
	m_currCamera = camera;
	for (unsigned int i = 0; i < 6; ++i)
	{
		m_voxelCache.beginCache(m_caches[i]);
		m_caches[i]->setOffset(xOffset, yOffset, zOffset);
	}

}

void ChunkRenderer::renderFace(VoxelCache::Face face, int x, int y, int z, int textureIdx, int color1, int color2, int color3, int color4)
{
	m_caches[face]->addFace(x, y, z, textureIdx, color1, color2, color3, color4);
}

void ChunkRenderer::end()
{
	m_voxelCache.beginRender(m_tileSet);
	for (unsigned int i = 0; i < 6; ++i)
	{
		m_voxelCache.endCache(m_caches[i]);
		m_voxelCache.renderCache(m_caches[i], *m_currCamera);
	}

	m_voxelCache.finishRender();
}