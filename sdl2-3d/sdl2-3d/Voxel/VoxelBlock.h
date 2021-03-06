#pragma once

/** Contains all block related structs / datatypes */
#include <vector>
#include <lua.hpp>
#include <LuaBridge.h>

struct Color8888;

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
	BlockPropertyValue(int valueBits, BlockPropertyValueType type) : valueBits(valueBits), type(type) {};
	int valueBits;
	BlockPropertyValueType type;

	bool operator==(const BlockPropertyValue& compare) const;
	bool operator>(const BlockPropertyValue& compare) const;
	bool operator>=(const BlockPropertyValue& compare) const;
	bool operator<(const BlockPropertyValue& compare) const;
	bool operator<=(const BlockPropertyValue& compare) const;
};

struct PerBlockProperty
{
	PerBlockProperty(luabridge::LuaRef ref) : ref(ref), defaultValue(0, LUA_INT)
	{};
	PerBlockProperty(const std::string& name, luabridge::LuaRef ref,
		BlockPropertyValueType type, int value)
		: name(name), ref(ref), defaultValue(value, type)
	{};

	/** Name of the table key holding the value */
	std::string name;
	/** Reference to lua table value */
	luabridge::LuaRef ref;
	BlockPropertyValue defaultValue;
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

/** The data stored for every block in a chunk */
__declspec(align(8)) struct VoxelBlock
{
	VoxelBlock() : id(0), blockDataIndex(-1), solid(false) {};
	//VoxelBlock(const VoxelBlock& copy) = delete;
	BlockID id;
	int blockDataIndex;
	unsigned solid : 1;
};

/** Global constant properties for all the blocks of a single type. */
struct BlockProperties
{
	BlockProperties(lua_State* state) : luaRef(state), blockUpdateMethod(state), hasBlockUpdateMethod(false) {}
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

	unsigned hasBlockUpdateMethod : 1;
	luabridge::LuaRef blockUpdateMethod;
	/** List of properties unique for every instance of this type */
	std::vector<PerBlockProperty> perBlockProperties;
	/** List of events that can be triggered to run lua scripts without polling lua */
	std::vector<BlockEventTrigger> events;
};