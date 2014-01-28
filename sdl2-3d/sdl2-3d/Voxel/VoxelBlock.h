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

enum Face
{
	TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3, FRONT = 4, BACK = 5
};

class PropertyManager;
struct LuaTableData;
class TextureManager;

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
	BlockProperties(luabridge::LuaRef luaRef, PropertyManager& propertyManager);
	TextureID getTextureID(Face face) const;

	/** Reference to the lua version of this block*/
	luabridge::LuaRef luaRef;

	unsigned short topTexture;
	unsigned short bottomTexture;
	unsigned short leftTexture;
	unsigned short rightTexture;
	unsigned short frontTexture;
	unsigned short backTexture;
	bool solid;

	/** List of properties unique for every instance of this type */
	std::vector<PerBlockProperty> perBlockProperties;
	/** List of events that can be triggered to run lua scripts without polling lua */
	std::vector<BlockEventTrigger> events;
};


#endif //VOXEL_BLOCK_H_
