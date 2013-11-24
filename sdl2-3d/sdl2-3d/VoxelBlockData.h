#ifndef VOXEL_BLOCK_DATA_H_
#define VOXEL_BLOCK_DATA_H_

#include "VoxelBlock.h"

typedef unsigned short BlockID;

struct VoxelBlockData
{
	BlockID blockId;
	unsigned int blockIndex;
};

#endif //VOXEL_BLOCK_DATA_H_