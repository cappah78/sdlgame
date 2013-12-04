#ifndef INVENTORY_BLOCK_H_
#define INVENTORY_BLOCK_H_

#include "VoxelBlock.h"

class InventoryBlock : public VoxelBlock<InventoryBlock>
{
public:
	//TODO: all the things

	static void onCreate(int x, int y, int z);
	static void onPlace(int x, int y, int z);
	static void onDestroy(int x, int y, int z);
	static void onBlockUpdate(int x, int y, int z);
};

#endif //INVENTORY_BLOCK_H_