#ifndef VOXEL_WORLD_H_
#define VOXEL_WORLD_H_

#include "VoxelBatch.h"
#include "VoxelChunk.h"

static const unsigned int CHUNK_LOAD_RANGE = 16;

class VoxelWorld
{
public:
	VoxelWorld();
	VoxelWorld(const VoxelWorld& copyMe);
	~VoxelWorld();
private:
	VoxelBatch batch;
	VoxelChunk chunk;
};

#endif //VOXEL_WORLD_H_