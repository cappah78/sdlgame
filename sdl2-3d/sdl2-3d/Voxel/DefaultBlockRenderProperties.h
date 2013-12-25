#ifndef DEFAULT_BLOCK_RENDER_PROPERTIES_H_
#define DEFAULT_BLOCK_RENDER_PROPERTIES_H_

#include <string>

class VoxelWorld;
struct LuaTableData;
class TextureManager;

struct DefaultBlockRenderProperties	// 8 bytes;
{
	DefaultBlockRenderProperties(LuaTableData& data, TextureManager& textureManager);

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

#endif //DEFAULT_BLOCK_RENDER_PROPERTIES_H_
