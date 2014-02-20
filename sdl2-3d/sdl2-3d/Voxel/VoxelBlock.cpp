#include "VoxelBlock.h"

#include "PropertyManager.h"
#include <assert.h>
#include <algorithm>

struct PropertiesSorter
{
	inline bool operator() (const PerBlockProperty& lhs, const PerBlockProperty& rhs)
	{
		return lhs.name < rhs.name;
	}
};

BlockProperties::BlockProperties(luabridge::LuaRef block, PropertyManager& propertyManager)
	: luaRef(block)
	, blockUpdateMethod(luaRef.state())
	, hasBlockUpdateMethod(false)
{
	luabridge::LuaRef texture = block["texture"];
	const char* topTexCStr = texture["top"];				//read the texture paths
	const char* bottomTexCStr = texture["bottom"];
	const char* leftTexCStr = texture["left"];
	const char* rightTexCStr = texture["right"];
	const char* frontTexCStr = texture["front"];
	const char* backTexCStr = texture["back"];

	solid = block["transparent"].isNil() ? block["transparent"] : true;

	assert(topTexCStr != NULL && "Top face texture not declared");
	assert(bottomTexCStr != NULL && "Bottom face texture not declared");
	assert(leftTexCStr != NULL && "Left face texture not declared");
	assert(rightTexCStr != NULL && "Right face texture not declared");
	assert(frontTexCStr != NULL && "Front face texture not declared");
	assert(backTexCStr != NULL && "Back face texture not declared");

	topTexture = propertyManager.getBlockTextureID(topTexCStr);
	bottomTexture = propertyManager.getBlockTextureID(bottomTexCStr);
	leftTexture = propertyManager.getBlockTextureID(leftTexCStr);
	rightTexture = propertyManager.getBlockTextureID(rightTexCStr);
	frontTexture = propertyManager.getBlockTextureID(frontTexCStr);
	backTexture = propertyManager.getBlockTextureID(backTexCStr);
}

unsigned short BlockProperties::getTextureID(Face face) const
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