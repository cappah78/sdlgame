#ifndef VOXEL_CHUNK_H_
#define VOXEL_CHUNK_H_

#include "VoxelCache.h"
#include "VoxelBlock.h"

#include <map>

/** Chunk size in one dimension */
static const unsigned int CHUNK_SIZE = 16;
static const unsigned int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
static const unsigned int CHUNK_SIZE_CUBED = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

typedef unsigned short BlockID;
typedef unsigned short BlockNumber;

struct VoxelBlockData	// 3 bytes
{
	BlockID blockId : 12;
	BlockNumber blockNumber : 12;
};

struct VoxelChunk
{
	VoxelChunk();
	VoxelChunk(const VoxelChunk& copyMe)
	{
		assert(false);
	};
	~VoxelChunk();

	float m_chunkX;
	float m_chunkY;
	float m_chunkZ;

	VoxelBlockData m_blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
};



#endif //VOXEL_CHUNK_H_