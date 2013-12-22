#ifndef PROPERTY_MANAGER_H_
#define PROPERTY_MANAGER_H_

#include "VoxelBlock.h"
#include "BlockRenderProperties.h"
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

private:
	/** ID counter used to give every registered block an unique id */
	BlockID m_lastRegisteredId;
	/** Maps Block name to its id */
	std::map<const std::string, BlockID> m_blockNameIDMap;
	/** Index: BlockID. Used to know what renderer to use for what block.*/
	std::vector<VoxelBlock::RenderType> m_renderTypes;

	//std::map<BlockID, DefaultBlock::DefaultBlockProperties> m_defaultBlockProperties;
	TextureManager& m_textureManager;

	LuaTableData getTableData(lua_State* const L, luabridge::LuaRef ref);
	void parseBlock(LuaTableData data);
};

#endif // PROPERTY_MANAGER_H_