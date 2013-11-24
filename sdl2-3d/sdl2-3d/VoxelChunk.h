#ifndef VOXEL_CHUNK_H_
#define VOXEL_CHUNK_H_

#include "VoxelCache.h"
#include "VoxelBlockData.h"

/** Chunk size in one dimension */
static const unsigned int CHUNK_SIZE = 16;
static const unsigned int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
static const unsigned int CHUNK_SIZE_CUBED = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

typedef unsigned char BlockInfo;

class VoxelChunk
{
public:
	VoxelChunk();
	VoxelChunk(const VoxelChunk& copyMe);
	~VoxelChunk();

	void initCaches(VoxelCache& voxelCache);

	void render(const Camera& camera, const TextureArray* tileSet, VoxelCache& voxelCache);

	void setBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ, VoxelBlockData block, bool updateChunk = true);
	VoxelBlockData getBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ);
private:
	bool m_isInit;
	float m_chunkX;
	float m_chunkY;
	float m_chunkZ;
	bool m_isUpdated;

	VoxelCache::Cache* m_topCache; 
	VoxelCache::Cache* m_bottomCache;
	VoxelCache::Cache* m_leftCache;
	VoxelCache::Cache* m_rightCache;
	VoxelCache::Cache* m_frontCache;
	VoxelCache::Cache* m_backCache;

	VoxelBlockData m_blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
};

#endif //VOXEL_CHUNK_H_