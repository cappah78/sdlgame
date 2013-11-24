#ifndef STONE_BLOCK_H_
#define STONE_BLOCK_H_

#include "VoxelBlock.h"

struct StoneBlockData
{
	int da;
	float fa;
	long derp;
};

class StoneBlock : public VoxelBlock<StoneBlock, StoneBlockData>
{
public:
	using VoxelBlock::VoxelBlock;
	using VoxelBlock::~VoxelBlock;

	//void onCreate();
	void onPlace();
	static void onDestroy(int blockX, int blockY, int blockZ);
	void onBlockUpdate();
};

#endif //STONE_BLOCK_H_