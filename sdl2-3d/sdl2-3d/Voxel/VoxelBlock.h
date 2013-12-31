#ifndef VOXEL_BLOCK_H_
#define VOXEL_BLOCK_H_

/** Contains all block related structs / datatypes */

#include "../Engine/Graphics/Color8888.h"
#include <string>
#include <vector>

typedef unsigned short BlockID;
typedef unsigned short TextureID;
typedef unsigned short BlockDataSize;
typedef Color8888 BlockColor;

class PropertyManager;
struct LuaTableData;
class TextureManager;

enum Face
{
	TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3, FRONT = 4, BACK = 5
};

struct BlockIDColor
{
	BlockIDColor() {};
	BlockIDColor(BlockID id, BlockColor color) : id(id), color(color) {};
	BlockID id;
	BlockColor color;
};

struct BlockIDColorSolid
{
	BlockID id;
	BlockColor color;
	bool isSolid;
};

static const char* const LUA_INT_NAME = "int";
static const char* const LUA_BOOL_NAME = "bool";
static const char* const LUA_FLOAT_NAME = "float";

enum BlockPropertyValueType
{
	LUA_INT, LUA_BOOL, LUA_FLOAT, UNKNOWN
};

struct PerBlockProperty
{
	PerBlockProperty() : type(LUA_INT) 
	{};
	PerBlockProperty(const std::string& name, BlockPropertyValueType type, int value) 
		: name(name), type(type), defaultValue(value) 
	{};

	std::string name;
	int defaultValue;	//will hold values of any type (including float bits/booleans)
	BlockPropertyValueType type;
};

struct PerBlockProperties
{
	PerBlockProperties() {};
	PerBlockProperties(const std::vector<PerBlockProperty>& p);
	unsigned int sizeBytes;
	std::vector<PerBlockProperty> properties;
};

struct BlockRenderData	// 8 bytes;
{
	BlockRenderData() {};
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

	TextureID getTextureID(Face face);
};

#endif //VOXEL_BLOCK_H_
