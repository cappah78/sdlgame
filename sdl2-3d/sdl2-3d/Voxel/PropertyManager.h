#ifndef PROPERTY_MANAGER_H_
#define PROPERTY_MANAGER_H_

#include "VoxelBlock.h"
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

	LuaTableData getTableData(luabridge::LuaRef ref);
	void parseBlock(LuaTableData blockData, BlockID blockID);
	void parseType(LuaTableData blockData, BlockID blockID);
	void parsePerBlockProperties(LuaTableData blockData, BlockID blockID);

	TextureManager& m_textureManager;

	/** ID counter used to give every registered block an unique id */
	BlockID m_lastRegisteredId;
	/** Maps Block name to its id */
	std::map<const std::string, BlockID> m_blockNameIDMap;

	/** Idx == blockID, value == isSolid */
	std::vector<bool> m_blockSolidity;
	/** Idx == blockID, value == has per block properties */
	std::vector<bool> m_hasPerBlockProperties;
	/** Idx == blockID */
	std::vector<BlockRenderData> m_blockRenderData;
	std::vector<PerBlockProperties> m_perBlockProperties;
};

#endif // PROPERTY_MANAGER_H_