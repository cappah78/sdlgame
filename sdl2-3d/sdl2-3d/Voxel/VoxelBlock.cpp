#include "VoxelBlock.h"

#include "PropertyManager.h"
#include "TextureManager.h"
#include <assert.h>
#include <algorithm>

struct PropertiesSorter
{
	inline bool operator() (const PerBlockProperty& lhs, const PerBlockProperty& rhs)
	{
		return lhs.name < rhs.name;
	}
};

PerBlockProperties::PerBlockProperties(const std::vector<PerBlockProperty>& p)
	: sizeBytes(0)
{
	properties.reserve(p.size());
	for (const PerBlockProperty& prop : p)
	{
		sizeBytes += 4;	// every property uses 4 bytes for now.
		properties.push_back(prop);
	}
	std::sort(properties.begin(), properties.end(), PropertiesSorter());	//sort alphabetically by key name
}

BlockRenderData::BlockRenderData(LuaTableData& data, TextureManager& textureManager)
{
	luabridge::LuaRef block = data.ref;
	luabridge::LuaRef texture = block["texture"];
	const char* topTexCStr = texture["top"];				//read the texture paths
	const char* bottomTexCStr = texture["bottom"];
	const char* leftTexCStr = texture["left"];
	const char* rightTexCStr = texture["right"];
	const char* frontTexCStr = texture["front"];
	const char* backTexCStr = texture["back"];

	isTransparent = block["transparent"].isNil() ? block["transparent"] : false;
	isAnimated = block["animated"].isNil() ? block["animated"] : false;

	assert(topTexCStr != NULL && "Top face texture not declared");
	assert(bottomTexCStr != NULL && "Bottom face texture not declared");
	assert(leftTexCStr != NULL && "Left face texture not declared");
	assert(rightTexCStr != NULL && "Right face texture not declared");
	assert(frontTexCStr != NULL && "Front face texture not declared");
	assert(backTexCStr != NULL && "Back face texture not declared");

	topTexture = textureManager.getTextureID(topTexCStr);
	bottomTexture = textureManager.getTextureID(bottomTexCStr);
	leftTexture = textureManager.getTextureID(leftTexCStr);
	rightTexture = textureManager.getTextureID(rightTexCStr);
	frontTexture = textureManager.getTextureID(frontTexCStr);
	backTexture = textureManager.getTextureID(backTexCStr);
}

TextureID BlockRenderData::getTextureID(Face face)
{
	switch (face)
	{
	case Face::TOP:
		return topTexture;
	case Face::BOTTOM:
		return bottomTexture;
	case Face::LEFT:
		return leftTexture;
	case Face::RIGHT:
		return rightTexture;
	case Face::FRONT:
		return frontTexture;
	case Face::BACK:
		return backTexture;
	default:
		assert(false && "invalid face id");
		return 0;
	}
}