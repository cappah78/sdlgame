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

BlockRenderData::BlockRenderData(luabridge::LuaRef block, TextureManager& textureManager)
{
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

TextureID BlockRenderData::getTextureID(Face face) const
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

bool BlockPropertyValue::operator==(const BlockPropertyValue& compare) const
{
	if (type == LUA_FLOAT)
	{
		float floatVal = *(float*) &valueBits;	//convert float bits to float
		if (compare.type == LUA_FLOAT)
		{
			float cmpFloatVal = *(float*) &compare.valueBits;
			return floatVal == cmpFloatVal;
		}
		return floatVal == (float) valueBits;
	}
	return valueBits == compare.valueBits;
}
bool BlockPropertyValue::operator>(const BlockPropertyValue& compare) const
{
	if (type == LUA_FLOAT)
	{
		float floatVal = *(float*) &valueBits;	//convert float bits to float
		if (compare.type == LUA_FLOAT)
		{
			float cmpFloatVal = *(float*) &compare.valueBits;
			return floatVal > cmpFloatVal;
		}
		return floatVal > (float) valueBits;
	}
	return valueBits > compare.valueBits;
}
bool BlockPropertyValue::operator>=(const BlockPropertyValue& compare) const
{
	if (type == LUA_FLOAT)
	{
		float floatVal = *(float*) &valueBits;	//convert float bits to float
		if (compare.type == LUA_FLOAT)
		{
			float cmpFloatVal = *(float*) &compare.valueBits;
			return floatVal >= cmpFloatVal;
		}
		return floatVal >= (float) valueBits;
	}
	return valueBits >= compare.valueBits;
}
bool BlockPropertyValue::operator<(const BlockPropertyValue& compare) const
{
	if (type == LUA_FLOAT)
	{
		float floatVal = *(float*) &valueBits;	//convert float bits to float
		if (compare.type == LUA_FLOAT)
		{
			float cmpFloatVal = *(float*) &compare.valueBits;
			return floatVal < cmpFloatVal;
		}
		return floatVal < (float) valueBits;
	}
	return valueBits < compare.valueBits;
}
bool BlockPropertyValue::operator<=(const BlockPropertyValue& compare) const
{
	if (type == LUA_FLOAT)
	{
		float floatVal = *(float*) &valueBits;	//convert float bits to float
		if (compare.type == LUA_FLOAT)
		{
			float cmpFloatVal = *(float*) &compare.valueBits;
			return floatVal <= cmpFloatVal;
		}
		return floatVal <= (float) valueBits;
	}
	return valueBits <= compare.valueBits;
}