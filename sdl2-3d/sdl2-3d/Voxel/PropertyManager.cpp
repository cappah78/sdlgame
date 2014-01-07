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

		m_blockProperties.resize(m_lastRegisteredId + 1, BlockProperties(block.state()));
		m_blockProperties.at(m_lastRegisteredId) = BlockProperties(block);

		parseBlock(m_blockProperties[m_lastRegisteredId]);

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
void PropertyManager::parseBlock(BlockProperties& properties)
{
	if (!properties.luaRef["perBlock"].isNil())
		parsePerBlockProperties(properties);
	if (!properties.luaRef["events"].isNil())
		parseEvents(properties);

	parseType(properties);
}

void PropertyManager::parseType(BlockProperties& properties)
{
	luabridge::LuaRef type = properties.luaRef["type"];
	std::string typeStr = type.tostring();

	if (typeStr.compare(DEFAULT_BLOCK)) {
		properties.renderData = BlockRenderData(properties.luaRef, m_textureManager);
		return;
	}

	assert(false && "Unrecognized/unimplemented block type");
}

int toInt(std::string str, BlockPropertyValueType type)
{
	float f;
	switch (type)
	{
	case LUA_INT:
		return atoi(str.c_str());
	case LUA_FLOAT:
		f = (float) atof(str.c_str());
		return *(int*) &f;
	case LUA_BOOL:
		return str == "true";
	default:
		return 0;
	}
}

void PropertyManager::parseEvents(BlockProperties& properties)
{
	luabridge::LuaRef events = properties.luaRef["events"];
	LuaTableData eventList = getTableData(events);

	for (std::pair<luabridge::LuaRef, luabridge::LuaRef> e : eventList.data)
	{
		//TODO: check not nil etc.
		luabridge::LuaRef triggerRef = e.second["trigger"];
		std::string triggerStr = triggerRef;
		triggerStr.erase(std::remove_if(triggerStr.begin(), triggerStr.end(), isspace), triggerStr.end());	//remove all spaces

		int leftEnd;
		int rightBegin;
		EventEvaluator eval;
		int idx = 0;
		//find evaluator and left/right value string split points
		for (auto it = triggerStr.begin(); it != triggerStr.end(); ++it, ++idx)	
		{
			char c = *it;
			if (c == '>')
			{
				//check >=
				char next = *(it + 1);
				if (next == '=')
				{
					leftEnd = idx;
					eval = GREATEREQUALS;
					rightBegin = idx + 2;
					break;
				}
				else
				{
					leftEnd = idx;
					eval = GREATER;
					rightBegin = idx + 1;
					break;
				}
			}
			else if (c == '<')
			{
				//check <=
				char next = *(it + 1);
				if (next == '=')
				{
					leftEnd = idx;
					eval = LESSEQUALS;
					rightBegin = idx + 2;
					break;
				}
				else
				{
					leftEnd = idx;
					eval = LESS;
					rightBegin = idx + 1;
					break;
				}
			}
			else if (c == '=')
			{
				//check ==
				char next = *(it + 1);
				if (next == '=')
				{
					leftEnd = idx;
					eval = EQUAL;
					rightBegin = idx + 2;
					break;
				}
				else
				{
					assert(false && "invalid expression, did you type = instead of == ?");
					break;
				}
			}
		}

		std::string left = triggerStr.substr(0, leftEnd);
		std::string right = triggerStr.substr(rightBegin, triggerStr.size());
		BlockPropertyValueType leftType = getValueType(left, properties);
		BlockPropertyValueType rightType = getValueType(right, properties);
		int leftValue = toInt(left, leftType);
		int rightValue = toInt(right, rightType);
		
		luabridge::LuaRef eventRef = e.second["event"];

		BlockEventTrigger trigger(leftValue, leftType, rightValue, rightType, eval, eventRef);

		assert(rightType != LUA_TICKCOUNTER && "Only left side of trigger may be a tickCount");	//TODO: fix?
		if (leftType == LUA_TICKCOUNTER)
		{
			assert(eval == EQUAL && "tickCount may only use == expression");
			assert(rightType == LUA_INT && "tickCount may only be compared with an integer type");
			properties.events.push_back(trigger);
			continue;
		}

		printf("trigger: %i, %i, %i \n", trigger.leftType, trigger.rightType, trigger.eval);
	}
}

BlockPropertyValueType PropertyManager::getValueType(std::string str, BlockProperties& properties)
{
	for (PerBlockProperty p : properties.properties)
		if (str == p.name)	//if value is equal to a property name, use the properties' type
			return p.type;

	if (str == "true" || str == "false")
		return LUA_BOOL;

	if (str == LUA_TICK_COUNT_NAME)
		return LUA_TICKCOUNTER;

	// check if contains a decimal point
	auto it = str.find('.');
	if (it != std::string::npos)
	{	//contains
		return LUA_FLOAT;
	}
	else
	{
		return LUA_INT;
	}
}

void PropertyManager::parsePerBlockProperties(BlockProperties& properties)
{
	luabridge::LuaRef perBlock = properties.luaRef["perBlock"];

	LuaTableData& perBlockLuaProperties = getTableData(perBlock);

	properties.properties.reserve(perBlockLuaProperties.data.size());
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
		// Get the default value
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
		printf("key: %s \n", key.c_str());
		properties.propertiesSizeBytes += 4;	//every property an int for now.
		properties.properties.push_back(PerBlockProperty(key, type, defaultVal));
	}
}