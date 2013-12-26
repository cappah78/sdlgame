#ifndef PROPERTY_MANAGER_H_
#define PROPERTY_MANAGER_H_

#include "VoxelBlock.h"
#include "BlockRenderData.h"
#include "TextureManager.h"

#include <assert.h>
#include <map>
#include <vector>

#include <lua.hpp>
#include <LuaBridge.h>

struct lua_State;

/** contains a list of key-value pairs, and the base reference */
struct LuaTableData
{
	typedef std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>> TableDataList;

	LuaTableData(luabridge::LuaRef ref) : ref(ref) {};
	LuaTableData(luabridge::LuaRef ref, TableDataList data) : ref(ref), data(data) {};
	luabridge::LuaRef ref;
	TableDataList data;
};


/** Used to keep track of constant data of blocks parsed from lua files */
class PropertyManager
{
public:
	PropertyManager(TextureManager& textureManager)
		: m_textureManager(textureManager) 
		, m_lastRegisteredId(0) 
	{};
	PropertyManager(const PropertyManager& copy) = delete;
	~PropertyManager() {};

	BlockID getBlockID(const std::string& blockName);
	BlockDataSize getBlockDataSize(BlockID blockID);

	BlockID registerBlockType(lua_State* const L, const std::string& blockname);

	BlockRenderData getBlockRenderData(BlockID blockID) const;

private:

	LuaTableData getTableData(lua_State* const L, luabridge::LuaRef ref);
	void parseBlock(LuaTableData data, BlockID blockID);

	TextureManager& m_textureManager;

	/** ID counter used to give every registered block an unique id */
	BlockID m_lastRegisteredId;
	/** Maps Block name to its id */
	std::map<const std::string, BlockID> m_blockNameIDMap;

	std::map<BlockID, BlockRenderData> m_blockRenderDataMap;
};

#endif // PROPERTY_MANAGER_H_