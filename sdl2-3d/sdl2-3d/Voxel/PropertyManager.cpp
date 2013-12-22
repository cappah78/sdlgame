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
		++m_lastRegisteredId;
		m_blockNameIDMap.insert(std::make_pair(blockname, m_lastRegisteredId));

		luabridge::LuaRef blocks = luabridge::getGlobal(L, LUA_BLOCKS_NAMESPACE);
		luabridge::LuaRef block = blocks[blockname];

		LuaTableData tableData = getTableData(L, block);

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

void PropertyManager::parseBlock(LuaTableData data)
{
	luabridge::LuaRef type = data.ref["type"];
	std::string typeStr = type.tostring();
	
	if (typeStr == DEFAULT_BLOCK) {
		BlockRenderProperties::DefaultRenderProperties defaultRenderProperties(data);
	}

	assert(false && "Unrecognized block type");
}
/*
void PropertyManager::parseDefault(luabridge::LuaRef block, const std::string& blockname, BlockID blockID)
{
	block["id"] = blockID;	//set the id field

	const luabridge::LuaRef& texture = block["texture"];	//grab the texture table
	const char* topTexCStr = texture["top"];				//read the texture paths
	const char* bottomTexCStr = texture["bottom"];
	const char* leftTexCStr = texture["left"];
	const char* rightTexCStr = texture["right"];
	const char* frontTexCStr = texture["front"];
	const char* backTexCStr = texture["back"];

	bool isTransparent;
	if (block["transparent"].isNil())
		isTransparent = false;	//default transparency is false
	else
		isTransparent = block["transparent"];

	bool isSolid;
	if (block["solid"].isNil())
		isSolid = false;	//default solidity is true
	else
		isSolid = block["solid"];

	assert(!block["strength"].isNil() && "Block strength not declared");
	assert(topTexCStr != NULL && "Top face texture not declared");
	assert(bottomTexCStr != NULL && "Bottom face texture not declared");
	assert(leftTexCStr != NULL && "Left face texture not declared");
	assert(rightTexCStr != NULL && "Right face texture not declared");
	assert(frontTexCStr != NULL && "Front face texture not declared");
	assert(backTexCStr != NULL && "Back face texture not declared");

	short blockStrength = block["strength"];

	TextureID topID = m_textureManager.getTextureID(topTexCStr);
	TextureID bottomID = m_textureManager.getTextureID(bottomTexCStr);
	TextureID leftID = m_textureManager.getTextureID(leftTexCStr);
	TextureID rightID = m_textureManager.getTextureID(rightTexCStr);
	TextureID frontID = m_textureManager.getTextureID(frontTexCStr);
	TextureID backID = m_textureManager.getTextureID(backTexCStr);

	DefaultBlock::DefaultBlockProperties properties = DefaultBlock::DefaultBlockProperties(
		topID, bottomID, leftID, rightID, frontID, backID, 
		isTransparent, isSolid, blockStrength);

	m_defaultBlockProperties.insert(std::make_pair(blockID, properties));
}
*/
BlockDataSize PropertyManager::getBlockDataSize(BlockID blockID)
{
	return 0;
}