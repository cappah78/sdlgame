#ifndef BLOCK_RENDER_DATA_H_
#define BLOCK_RENDER_DATA_H_

#include <string>

class VoxelWorld;
struct LuaTableData;
class TextureManager;

struct BlockRenderData	// 8 bytes;
{
	BlockRenderData(LuaTableData& data, TextureManager& textureManager);

	unsigned topTexture : 10;
	unsigned bottomTexture : 10;
	unsigned leftTexture : 10;
	unsigned rightTexture : 10;
	unsigned frontTexture : 10;
	unsigned backTexture : 10;
	unsigned isTransparent : 1;
	unsigned isAnimated : 1;
	unsigned padding : 2;
};

#endif //BLOCK_RENDER_DATA_H_
