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
	voxelCache.renderCache(m_topCache, tileSet, camera);
	voxelCache.renderCache(m_bottomCache, tileSet, camera);
	voxelCache.renderCache(m_leftCache, tileSet, camera);
	voxelCache.renderCache(m_rightCache, tileSet, camera);
	voxelCache.renderCache(m_frontCache, tileSet, camera);
	voxelCache.renderCache(m_backCache, tileSet, camera);
}

void VoxelChunk::initCaches(VoxelCache& voxelCache)
{
	float xOffset = float(chunkX);
	float yOffset = float(chunkY);
	float zOffset = float(chunkZ);

	m_topCache = voxelCache.createCache(VoxelCache::TOP, xOffset, yOffset, zOffset);
	m_bottomCache = voxelCache.createCache(VoxelCache::BOTTOM, xOffset, yOffset, zOffset);
	m_leftCache = voxelCache.createCache(VoxelCache::LEFT, xOffset, yOffset, zOffset);
	m_rightCache = voxelCache.createCache(VoxelCache::RIGHT, xOffset, yOffset, zOffset);
	m_frontCache = voxelCache.createCache(VoxelCache::FRONT, xOffset, yOffset, zOffset);
	m_backCache = voxelCache.createCache(VoxelCache::BACK, xOffset, yOffset, zOffset);
}

void VoxelChunk::setBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockId blockId)
{
	isUpdated = false;
	m_blockIds[blockX * CHUNK_SIZE_SQUARED + blockY * CHUNK_SIZE + blockZ] = blockId;
}

void VoxelChunk::setBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockId blockId, BlockInfo info)
{
	isUpdated = false;
	unsigned int idx = blockX * CHUNK_SIZE_SQUARED + blockY * CHUNK_SIZE + blockZ;
	m_blockIds[idx] = blockId;
	m_blockInfos[idx] = info;
}

void VoxelChunk::setBlockInfo(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockInfo info, bool updateChunk)
{
	if (updateChunk)
		isUpdated = false;
	m_blockInfos[blockX * CHUNK_SIZE_SQUARED + blockY * CHUNK_SIZE + blockZ] = info;
}

