#ifndef VOXEL_BLOCK_H_
#define VOXEL_BLOCK_H_

#include "../Engine/Graphics/Color8888.h"

typedef unsigned short BlockID;
typedef unsigned short BlockDataSize;
typedef Color8888 BlockColor;

struct BlockIDColor
{
	BlockIDColor() {};
	BlockIDColor(BlockID id, BlockColor color) : id(id), color(color) {};
	BlockID id;
	BlockColor color;
};

#endif //VOXEL_BLOCK_H_
