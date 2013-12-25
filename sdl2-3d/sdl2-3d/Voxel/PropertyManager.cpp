#include "PropertyManager.h"

#include "LuaVoxelConfig.h"

#include <lua.hpp>
#include <LuaBridge.h>

#include <iostream>
#include <stdio.h>

#include <unordered_map>

static const char* LUA_BLOCKS_NAMESPACE = "Blocks";
static const std::string DEFAULT_BLOCK("defaultblock");

BlockID PropertyManager::getBlockID(const std::string& blockName)
{
	return m_blockNameIDMap.at(blockName);
}

BlockID PropertyManager::registerBlockType(lua_State* const L, const std::string& blockname)
{
	auto it = m_blockNameIDMap.find(blockname);
	if (it != m_blockNameIDMap.end())
	{	//already contained
		std::cout << "Blocktype already registered: " << blockname << std::endl;
		return it->second;
	}
	else
	{
		//TODO: some sort of persistent block id manager thing.
		++m_lastRegisteredId;
		m_blockNameIDMap.insert(std::make_pair(blockname, m_lastRegisteredId));

		luabridge::LuaRef blocks = luabridge::getGlobal(L, LUA_BLOCKS_NAMESPACE);
		luabridge::LuaRef block = blocks[blockname];
		block["id"] = m_lastRegisteredId;

		LuaTableData tableData = getTableData(L, block);

		parseBlock(tableData, m_lastRegisteredId);

		return m_lastRegisteredId;
	}
}

LuaTableData PropertyManager::getTableData(lua_State* const L, luabridge::LuaRef ref)
{
	LuaTableData properties(ref);

	ref.push(L);
	luabridge::push(L, luabridge::Nil());
	while (lua_next(L, -2))
	{
		luabridge::LuaRef& key = luabridge::LuaRef::fromStack(L, -2);
		luabridge::LuaRef& val = luabridge::LuaRef::fromStack(L, -1);
		properties.data.push_back(std::make_pair(key, val));
		lua_pop(L, 1);
	}

	for (auto pair : properties.data) {
		std::cout << (pair.first) << ":" << (pair.second) << std::endl;
	}

	return properties;
}

void PropertyManager::parseBlock(LuaTableData data, BlockID blockID)
{
	luabridge::LuaRef type = data.ref["type"];
	std::string typeStr = type.tostring();

	if (typeStr.compare(DEFAULT_BLOCK)) {
		DefaultBlockRenderProperties properties(data, m_textureManager);
		m_defaultBlockProperties.insert(std::make_pair(blockID, properties));
		if (m_renderTypes.size() < (unsigned int) blockID + 1)
			m_renderTypes.resize((int) (blockID * 1.5f + 1));
		m_renderTypes[blockID] = BlockRenderType::DEFAULT;
		return;
	}

	assert(false && "Unrecognized block type");
}

DefaultBlockRenderProperties PropertyManager::getDefaultRenderProperties(BlockID blockID) const
{
	return m_defaultBlockProperties.at(blockID);
}

BlockRenderType PropertyManager::getRenderType(BlockID blockID) const
{
	return m_renderTypes.at(blockID);
}

BlockDataSize PropertyManager::getBlockDataSize(BlockID blockID)
{
	return 0;
}