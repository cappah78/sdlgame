#ifndef BLOCK_RENDER_DATA_H_
#define BLOCK_RENDER_DATA_H_

#include <string>
#include "VoxelRenderer.h"
#include "TextureManager.h"

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

	TextureID getTextureID(VoxelRenderer::Face face)
	{
		switch (face)
		{
		case VoxelRenderer::Face::TOP:
			return topTexture;
			break;
		case VoxelRenderer::Face::BOTTOM:
			return bottomTexture;
			break;
		case VoxelRenderer::Face::LEFT:
			return leftTexture;
			break;
		case VoxelRenderer::Face::RIGHT:
			return rightTexture;
			break;
		case VoxelRenderer::Face::FRONT:
			return frontTexture;
			break;
		case VoxelRenderer::Face::BACK:
			return backTexture;
			break;
		default:
			return 0;
			assert(false && "invalid face id");
			break;
		}
	}
};

#endif //BLOCK_RENDER_DATA_H_
