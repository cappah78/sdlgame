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

void VoxelChunk::setBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockId blockId)
{
	isUpdated = false;
	blockIds[blockX * CHUNK_SIZE_SQUARED + blockY * CHUNK_SIZE + blockZ] = blockId;
}

void VoxelChunk::setBlock(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockId blockId, BlockInfo info)
{
	isUpdated = false;
	unsigned int idx = blockX * CHUNK_SIZE_SQUARED + blockY * CHUNK_SIZE + blockZ;
	blockIds[idx] = blockId;
	blockInfos[idx] = info;
}

void VoxelChunk::setBlockInfo(unsigned int blockX, unsigned int blockY, unsigned int blockZ, BlockInfo info, bool updateChunk)
{
	if (updateChunk)
		isUpdated = false;
	blockInfos[blockX * CHUNK_SIZE_SQUARED + blockY * CHUNK_SIZE + blockZ] = info;
}

