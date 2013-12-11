#ifndef VOXEL_CHUNK_H_
#define VOXEL_CHUNK_H_

#include "VoxelCache.h"

#include <vector>

/** Chunk size in one dimension */
static const unsigned int CHUNK_SIZE = 16;
static const unsigned int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
static const unsigned int CHUNK_SIZE_CUBED = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

typedef unsigned short BlockID;

struct VoxelBlockData
{
	BlockID m_blockId;
	unsigned int m_dataIndex;
};

struct VoxelChunk
{
	VoxelChunk(int x, int y, int z)
		: m_chunkX(x), m_chunkY(y), m_chunkZ(z)
	{};
	VoxelChunk(const VoxelChunk& copyMe) = delete;
	~VoxelChunk() {};

	int m_chunkX;
	int m_chunkY;
	int m_chunkZ;

	//template <struct T>
	//T* getData<T>(unsigned int dataIndex);

	VoxelBlockData m_blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	std::vector<unsigned char> m_blockData;
};

#endif //VOXEL_CHUNK_H_