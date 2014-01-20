#ifndef VOXEL_BLOCK_H_
#define VOXEL_BLOCK_H_

/** Contains all block related structs / datatypes */

#include "../Game.h"
#include "../Engine/Graphics/Color8888.h"
#include <string>
#include <vector>
#include <lua.hpp>
#include <LuaBridge.h>

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

//TODO: perhaps move to a .lua config
static const char* const LUA_BLOCKS_NAMESPACE = "Blocks";
static const char* const LUA_INT_NAME = "int";
static const char* const LUA_BOOL_NAME = "bool";
static const char* const LUA_FLOAT_NAME = "float";
static const char* const LUA_TICK_COUNT_NAME = "tickCount";
static const char* const LUA_BLOCK_X_NAME = "x";
static const char* const LUA_BLOCK_Y_NAME = "y";
static const char* const LUA_BLOCK_Z_NAME = "z";

static const std::string DEFAULT_BLOCK = "defaultblock";


enum BlockPropertyValueType
{
	LUA_INT, LUA_BOOL, LUA_FLOAT, LUA_TICKCOUNTER, LUA_PROPERTY_REF, LUA_BLOCK_X, LUA_BLOCK_Y, LUA_BLOCK_Z
};

enum EventEvaluator
{
	EQUAL, LESS, GREATER, LESSEQUALS, GREATEREQUALS
};

struct BlockPropertyValue
{
	BlockPropertyValue(int value, BlockPropertyValueType type) : value(value), type(type) {};
	int value;
	BlockPropertyValueType type;

	bool operator==(const BlockPropertyValue& compare) const;
	bool operator>(const BlockPropertyValue& compare) const;
	bool operator>=(const BlockPropertyValue& compare) const;
	bool operator<(const BlockPropertyValue& compare) const;
	bool operator<=(const BlockPropertyValue& compare) const;
};

struct PerBlockProperty
{
	PerBlockProperty(luabridge::LuaRef ref) : ref(ref), prop(0, LUA_INT)
	{};
	PerBlockProperty(const std::string& name, luabridge::LuaRef ref,
		BlockPropertyValueType type, int value)
		: name(name), ref(ref), prop(value, type)
	{};

	/** Name of the table key holding the value */
	std::string name;
	/** Reference to lua table value */
	luabridge::LuaRef ref;
	BlockPropertyValue prop;
};

/** A single event trigger for a block type */
struct BlockEventTrigger
{
	BlockEventTrigger(BlockPropertyValue left, BlockPropertyValue right, EventEvaluator eval, luabridge::LuaRef process)
		: left(left)
		, right(right)
		, eval(eval)
		, process(process)
	{}
		
	BlockPropertyValue left;
	BlockPropertyValue right;
	EventEvaluator eval;

	luabridge::LuaRef process;

	bool isTriggered() const
	{
		switch (eval)
		{
		case EventEvaluator::EQUAL:
			return (left == right);
		case EventEvaluator::GREATER:
			return (left > right);
		case EventEvaluator::GREATEREQUALS:
			return (left >= right);
		case EventEvaluator::LESS:
			return (left < right);
		case EventEvaluator::LESSEQUALS:
			return (left <= right);
		default:
			assert(false);
			return false;
		}
	}
};

struct BlockRenderData	// 8 bytes;
{
	BlockRenderData() {};
	/** Parses the properties from the given table */
	BlockRenderData(luabridge::LuaRef block, TextureManager& textureManager);

	unsigned topTexture : 10;
	unsigned bottomTexture : 10;
	unsigned leftTexture : 10;
	unsigned rightTexture : 10;
	unsigned frontTexture : 10;
	unsigned backTexture : 10;
	unsigned isTransparent : 1;
	unsigned isAnimated : 1;
	unsigned padding : 2;

	TextureID getTextureID(Face face) const;
};

/** The data stored for every block in a chunk */
__declspec(align(16)) struct VoxelBlock
{
	VoxelBlock() : id(0), blockDataIndex(-1), skyVisible(false), solid(false), lightLevel(0), update(false) {};
	//VoxelBlock(const VoxelBlock& copy) = delete;
	BlockID id;
	BlockColor color;
	int blockDataIndex;
	unsigned skyVisible : 1;
	unsigned solid : 1;
	/** true if should run onBlockUpdate */
	unsigned update : 1;
	unsigned lightLevel : 4;
};

/** Global constant properties for all the blocks of a single type. */
struct BlockProperties
{
	BlockProperties(luabridge::LuaRef luaRef) : luaRef(luaRef) {}

	/** Reference to the lua version of this block*/
	luabridge::LuaRef luaRef;
	/** Stuff to tell how to render instances of this block */
	BlockRenderData renderData;
	/** If this block is not transparent */
	unsigned solid : 1;

	/** List of properties unique for every instance of this type */
	std::vector<PerBlockProperty> perBlockProperties;
	/** List of events that can be triggered to run lua scripts without polling lua */
	std::vector<BlockEventTrigger> events;
};

#endif //VOXEL_BLOCK_H_
