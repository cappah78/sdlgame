#ifndef PROPERTY_MANAGER_H_
#define PROPERTY_MANAGER_H_

#include "VoxelBlock.h"
#include "DefaultBlock.h"

#include "TextureManager.h"

#include <assert.h>
#include <map>

#include <lua.hpp>
#include <LuaBridge.h>

struct lua_State;

/** Used to keep track of constant data of blocks parsed from lua files */
class PropertyManager
{
public:
	PropertyManager(TextureManager& textureManager)
		: m_textureManager(textureManager) 
		, m_lastRegisteredId(0) 
	{};
	~PropertyManager() {};

	BlockID registerBlockType(lua_State* const L, const std::string& blockname);

	DefaultBlock::DefaultBlockProperties getDefaultBlockProperties(BlockID blockID);

	BlockID getBlockID(const std::string& blockName);
private:
	void parseProperties(lua_State* const L, const std::string& blockname, BlockID blockID);

	void parseDefault(luabridge::LuaRef block, const std::string& blockname, BlockID blockID);

	BlockID m_lastRegisteredId;
	std::map<const std::string, BlockID> m_blockNameIDMap;
	std::map<BlockID, DefaultBlock::DefaultBlockProperties> m_defaultBlockProperties;

	TextureManager& m_textureManager;
};

#endif // PROPERTY_MANAGER_H_