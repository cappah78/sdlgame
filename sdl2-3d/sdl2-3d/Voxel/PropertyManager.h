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
	{
		m_blockSolidity.push_back(false);	//blockid 0 is not solid
	};
	PropertyManager(const PropertyManager& copy) = delete;
	~PropertyManager() {};

	BlockID registerBlockType(lua_State* const L, const std::string& blockname);

	inline BlockID getBlockID(const std::string& blockName)	const		{ return m_blockNameIDMap.at(blockName); };
	inline bool hasPerBlockProperties(BlockID blockID) const			{ return m_hasPerBlockProperties.at(blockID); };
	inline PerBlockProperties getPerBlockProperties(BlockID blockID)	{ assert(m_hasPerBlockProperties.at(blockID)); return m_perBlockProperties.at(blockID); };
	inline BlockRenderData getBlockRenderData(BlockID blockID) const	{ return m_blockRenderData.at(blockID); };
	inline luabridge::LuaRef getLuaBlockRef(BlockID blockID) const		{ return m_luaBlockRefs.at(blockID); };
	inline bool isSolid(BlockID blockID) const							{ return m_blockSolidity.at(blockID); };

private:

	LuaTableData getTableData(luabridge::LuaRef ref) const;
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
	/** idx == blockID, value luarefs to access the lua values of this block */
	std::vector<luabridge::LuaRef> m_luaBlockRefs;

};

#endif // PROPERTY_MANAGER_H_