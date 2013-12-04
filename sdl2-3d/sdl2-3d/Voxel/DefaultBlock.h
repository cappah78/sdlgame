#ifndef DEFAULT_BLOCK_H_
#define DEFAULT_BLOCK_H_

#include "VoxelBlock.h"
#include <string>

/** Basic unanimated block */
class DefaultBlock : public VoxelBlock<DefaultBlock>
{
public:
	struct DefaultBlockProperties	// 9 bytes;
	{
		DefaultBlockProperties(
		short topTex,
		short bottomTex,
		short leftTex,
		short rightTex,
		short frontTex,
		short backTex,
		bool transparant,
		bool solid,
		short strength)
		: topTexture(topTex)
		, bottomTexture(bottomTex)
		, leftTexture(leftTex)
		, rightTexture(rightTex)
		, frontTexture(frontTex)
		, backTexture(backTex)
		, isTranspant(transparant)
		, isSolid(solid)
		, strength(strength)
		{};

		unsigned topTexture : 10;
		unsigned bottomTexture : 10;
		unsigned leftTexture : 10;
		unsigned rightTexture : 10;
		unsigned frontTexture : 10;
		unsigned backTexture : 10;
		unsigned isTranspant : 1;
		unsigned isSolid : 1;
		unsigned strength : 10;
	};

	using VoxelBlock::VoxelBlock;
	using VoxelBlock::~VoxelBlock;

	void onCreate(VoxelWorld& world, int x, int y, int z);
	void onPlace(VoxelWorld& world, int x, int y, int z);
	void onDestroy(VoxelWorld& world, int x, int y, int z);
	void onBlockUpdate(VoxelWorld& world, int x, int y, int z);

	const DefaultBlockProperties& getProperties() { return m_properties; };
private:
	DefaultBlockProperties m_properties;
};

#endif //DEFAULT_BLOCK_H_
