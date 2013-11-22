#ifndef VOXEL_CHUNK_H_
#define VOXEL_CHUNK_H_

#include "VoxelCache.h"

/** Chunk size in one dimension */
static const unsigned int CHUNK_SIZE = 16;
static const unsigned int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
static const unsigned int CHUNK_SIZE_CUBED = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

typedef unsigned short BlockId;
typedef unsigned char BlockInfo;

class VoxelChunk
{
public:
	VoxelChunk();
	VoxelChunk(const VoxelChunk& copyMe);
	~VoxelChunk();

	void initCaches(VoxelCache& voxelCache);

	void render(const Camera& camera, const TextureArray* tileSet, VoxelCache& voxelCache);

	void setBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockId blockId);
	void setBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockId blockId, BlockInfo info);
	void setBlockInfo(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockInfo info, bool updateChunk);
	BlockId getBlockId(unsigned int blockX, unsigned int blockY);
	BlockInfo getBlockInfo(unsigned int blockX, unsigned int blockY);
private:
	int chunkX;
	int chunkY;
	int chunkZ;
	bool isUpdated;

	VoxelCache::Cache* m_topCache; 
	VoxelCache::Cache* m_bottomCache;
	VoxelCache::Cache* m_leftCache;
	VoxelCache::Cache* m_rightCache;
	VoxelCache::Cache* m_frontCache;
	VoxelCache::Cache* m_backCache;

	BlockId m_blockIds[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	BlockInfo m_blockInfos[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
};

#endif //VOXEL_CHUNK_H_