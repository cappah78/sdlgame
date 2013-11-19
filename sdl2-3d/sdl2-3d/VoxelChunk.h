#ifndef VOXEL_CHUNK_H_
#define VOXEL_CHUNK_H_

/** Chunk size in one dimension */
static const unsigned int CHUNK_SIZE = 16;
static const unsigned int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
static const unsigned int CHUNK_SIZE_SQUARED3 = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

typedef unsigned short BlockId;
typedef unsigned char BlockInfo;

class VoxelChunk
{
public:
	VoxelChunk();
	VoxelChunk(const VoxelChunk& copyMe);
	~VoxelChunk();

	void setBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockId blockId);
	void setBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockId blockId, BlockInfo info);
	void setBlockInfo(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockInfo info, bool updateChunk);
	BlockId getBlockId(unsigned int blockX, unsigned int blockY);
	BlockInfo getBlockInfo(unsigned int blockX, unsigned int blockY);
private:
	int chunkX;
	int chunkY;
	bool isUpdated;

	BlockId blockIds[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	BlockInfo blockInfos[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
};

#endif //VOXEL_CHUNK_H_