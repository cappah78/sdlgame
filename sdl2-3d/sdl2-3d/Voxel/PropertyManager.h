#ifndef PROPERTY_MANAGER_H_
#define PROPERTY_MANAGER_H_

#include "VoxelBlock.h"
#include "DefaultBlockRenderProperties.h"
#include "TextureManager.h"

#include <assert.h>
#include <map>
#include <vector>

#include <lua.hpp>
#include <LuaBridge.h>

struct lua_State;


struct LuaTableData
{
	typedef std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>> TableDataList;

	LuaTableData(luabridge::LuaRef ref) : ref(ref) {};
	LuaTableData(luabridge::LuaRef ref, TableDataList data) : ref(ref), data(data) {};
	luabridge::LuaRef ref;
	TableDataList data;
};

enum BlockRenderType
{
	DEFAULT = 0, MESH = 1
};

/** Used to keep track of constant data of blocks parsed from lua files */
class PropertyManager
{
public:
	PropertyManager(TextureManager& textureManager)
		: m_textureManager(textureManager) 
		, m_lastRegisteredId(0) 
		, m_renderTypes(0)
	{};
	PropertyManager(const PropertyManager& copy) = delete;
	~PropertyManager() {};

	BlockID getBlockID(const std::string& blockName);
	BlockDataSize getBlockDataSize(BlockID blockID);

	BlockID registerBlockType(lua_State* const L, const std::string& blockname);

	BlockRenderType getRenderType(BlockID blockID) const;
	DefaultBlockRenderProperties getDefaultRenderProperties(BlockID blockID) const;

private:

	LuaTableData getTableData(lua_State* const L, luabridge::LuaRef ref);
	void parseBlock(LuaTableData data, BlockID blockID);

	TextureManager& m_textureManager;

	/** ID counter used to give every registered block an unique id */
	BlockID m_lastRegisteredId;
	/** Maps Block name to its id */
	std::map<const std::string, BlockID> m_blockNameIDMap;


	/** Index: BlockID. Used to know what renderer to use for what block.*/
	std::vector<BlockRenderType> m_renderTypes;

	std::map<BlockID, DefaultBlockRenderProperties> m_defaultBlockProperties;
};

#endif // PROPERTY_MANAGER_H_