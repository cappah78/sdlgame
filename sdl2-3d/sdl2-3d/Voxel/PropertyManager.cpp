#include "PropertyManager.h"

#include "LuaVoxelConfig.h"

#include <lua.hpp>
#include <LuaBridge.h>

#include <unordered_map>
#include <algorithm>

static const char* LUA_BLOCKS_NAMESPACE = "Blocks";
static const std::string DEFAULT_BLOCK("defaultblock");

BlockID PropertyManager::registerBlockType(lua_State* const L, const std::string& blockname)
{
	auto it = m_blockNameIDMap.find(blockname);
	if (it != m_blockNameIDMap.end())
	{	//already contained
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

		m_luaBlockRefs.resize(m_lastRegisteredId + 1, luabridge::LuaRef(L));
		m_luaBlockRefs.at(m_lastRegisteredId) = block;

		LuaTableData tableData = getTableData(block);
		parseBlock(tableData, m_lastRegisteredId);

		return m_lastRegisteredId;
	}
}

LuaTableData PropertyManager::getTableData(luabridge::LuaRef ref) const
{
	LuaTableData properties(ref);

	lua_State* L = ref.state();

	ref.push(L);
	luabridge::push(L, luabridge::Nil());
	while (lua_next(L, -2))
	{
		luabridge::LuaRef key = luabridge::LuaRef::fromStack(L, -2);
		luabridge::LuaRef val = luabridge::LuaRef::fromStack(L, -1);
		properties.data.push_back(std::make_pair(key, val));
		lua_pop(L, 1);
	}

	return properties;
}
void PropertyManager::parseBlock(LuaTableData blockData, BlockID blockID)
{
	if (!blockData.ref["perBlock"].isNil())
		parsePerBlockProperties(blockData, blockID);

	parseType(blockData, blockID);
}

void PropertyManager::parseType(LuaTableData blockData, BlockID blockID)
{
	luabridge::LuaRef type = blockData.ref["type"];
	std::string typeStr = type.tostring();

	if (typeStr.compare(DEFAULT_BLOCK)) {
		BlockRenderData renderData(blockData, m_textureManager);
		m_blockRenderData.resize(blockID + 1);
		m_blockRenderData.at(blockID) = renderData;
		m_blockSolidity.resize(blockID + 1, false);
		m_blockSolidity.at(blockID) = !renderData.isTransparent;
		return;
	}

	assert(false && "Unrecognized/unimplemented block type");
}

void PropertyManager::parsePerBlockProperties(LuaTableData blockData, BlockID blockID)
{
	luabridge::LuaRef perBlock = blockData.ref["perBlock"];

	LuaTableData& perBlockLuaProperties = getTableData(perBlock);

	std::vector<PerBlockProperty> propertyList;
	propertyList.reserve(perBlockLuaProperties.data.size());
	for (std::pair<luabridge::LuaRef, luabridge::LuaRef> r : perBlockLuaProperties.data)
	{
		//TODO: error checking.
		std::string key(r.first);	// name of the lua variable
		std::string value(r.second);// value of the lua variable
		unsigned pos = value.find("(");
		unsigned pos2 = value.find(")");
		std::string valueType = value.substr(0, pos); //value before ( and )
		std::string valueValue = value.substr(pos + 1, (pos2 - pos - 1)); //get value in between ( and )

		BlockPropertyValueType type;
		int defaultVal = NULL;

		// Parse all the different possible types
		if (valueType == LUA_INT_NAME)
		{
			type = BlockPropertyValueType::LUA_INT;
			defaultVal = std::atoi(valueValue.c_str());
			perBlock[key] = (int) defaultVal;
		}
		else if (valueType == LUA_BOOL_NAME)
		{
			type = BlockPropertyValueType::LUA_BOOL;
			defaultVal = (int) (valueValue == "true");
		}
		else if (valueType == LUA_FLOAT_NAME)
		{
			type = BlockPropertyValueType::LUA_FLOAT;
			float f = (float) atof(valueValue.c_str());
			defaultVal = *(int*) &f;
		}
		else
		{
			assert(false && "Unsupported type");
		}
		propertyList.push_back(PerBlockProperty(key, type, defaultVal));
	}

	if (propertyList.size() > 0)
	{
		m_perBlockProperties.resize(blockID + 1);
		m_hasPerBlockProperties.at(blockID) = true;
		m_perBlockProperties.at(blockID) = PerBlockProperties(propertyList);
	}

}