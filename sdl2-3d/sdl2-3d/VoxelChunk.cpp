#include "VoxelChunk.h"

VoxelChunk::VoxelChunk()
{

}

VoxelChunk::VoxelChunk(const VoxelChunk& copyMe)
{

}

VoxelChunk::~VoxelChunk()
{

}

void VoxelChunk::render(const Camera& camera, const TextureArray* tileSet, VoxelCache& voxelCache)
{
	assert(m_isInit);
	voxelCache.renderCache(m_topCache, tileSet, camera);
	voxelCache.renderCache(m_bottomCache, tileSet, camera);
	voxelCache.renderCache(m_leftCache, tileSet, camera);
	voxelCache.renderCache(m_rightCache, tileSet, camera);
	voxelCache.renderCache(m_frontCache, tileSet, camera);
	voxelCache.renderCache(m_backCache, tileSet, camera);
}

void VoxelChunk::initCaches(VoxelCache& voxelCache)
{
	assert(!m_isInit);
	m_topCache = voxelCache.createCache(VoxelCache::TOP, m_chunkX, m_chunkY, m_chunkZ);
	m_bottomCache = voxelCache.createCache(VoxelCache::BOTTOM, m_chunkX, m_chunkY, m_chunkZ);
	m_leftCache = voxelCache.createCache(VoxelCache::LEFT, m_chunkX, m_chunkY, m_chunkZ);
	m_rightCache = voxelCache.createCache(VoxelCache::RIGHT, m_chunkX, m_chunkY, m_chunkZ);
	m_frontCache = voxelCache.createCache(VoxelCache::FRONT, m_chunkX, m_chunkY, m_chunkZ);
	m_backCache = voxelCache.createCache(VoxelCache::BACK, m_chunkX, m_chunkY, m_chunkZ);
	m_isInit = true;
}

void VoxelChunk::setBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ, VoxelBlockData block, bool updateChunk)
{
	m_isUpdated = updateChunk;
	unsigned int idx = blockX * CHUNK_SIZE_SQUARED + blockY * CHUNK_SIZE + blockZ;
	m_blocks[idx] = block;
}

VoxelBlockData VoxelChunk::getBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ)
{
	unsigned int idx = blockX * CHUNK_SIZE_SQUARED + blockY * CHUNK_SIZE + blockZ;
	return m_blocks[idx];
}