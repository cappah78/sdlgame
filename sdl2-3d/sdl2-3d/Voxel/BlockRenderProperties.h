#ifndef DEFAULT_BLOCK_H_
#define DEFAULT_BLOCK_H_

#include <string>

class VoxelWorld;
struct LuaTableData;

/** Basic unanimated block */
class BlockRenderProperties
{
public:
	struct DefaultRenderProperties	// 8 bytes;
	{
		DefaultRenderProperties(LuaTableData& data);

		DefaultRenderProperties(
		short topTex,
		short bottomTex,
		short leftTex,
		short rightTex,
		short frontTex,
		short backTex,
		bool transparant)
		: topTexture(topTex)
		, bottomTexture(bottomTex)
		, leftTexture(leftTex)
		, rightTexture(rightTex)
		, frontTexture(frontTex)
		, backTexture(backTex)
		, isTranspant(transparant)
		{};

		unsigned topTexture : 10;
		unsigned bottomTexture : 10;
		unsigned leftTexture : 10;
		unsigned rightTexture : 10;
		unsigned frontTexture : 10;
		unsigned backTexture : 10;
		unsigned isTranspant : 1;
	};
private:

};

#endif //DEFAULT_BLOCK_H_
