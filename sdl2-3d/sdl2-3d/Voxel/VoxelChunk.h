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
	BlockID blockId;
	void* dataPointer;
};

struct VoxelChunk
{
	VoxelChunk();
	VoxelChunk(const VoxelChunk& copyMe) = delete;
	~VoxelChunk();

	float m_chunkX;
	float m_chunkY;
	float m_chunkZ;

	VoxelBlockData m_blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	std::vector<unsigned char> m_blockData;
};

#endif //VOXEL_CHUNK_H_