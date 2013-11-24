#ifndef DIRT_BLOCK_H_
#define DIRT_BLOCK_H_

#include "VoxelBlock.h"

struct DirtBlockData
{
	DirtBlockData(){};
	int da;
	float fa;
	long derp;
};

class DirtBlock : public VoxelBlock<DirtBlock, DirtBlockData>
{
public:
	using VoxelBlock::VoxelBlock;
	using VoxelBlock::~VoxelBlock;

	void onCreate();
	static void onDestroy(int blockX, int blockY, int blockZ);
	void onBlockUpdate();
	static unsigned int da;
};

#endif //DIRT_BLOCK_H_