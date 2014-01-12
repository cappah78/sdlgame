#include "PropertyManager.h"

#include "LuaVoxelConfig.h"

#include <lua.hpp>
#include <LuaBridge.h>

#include <unordered_map>
#include <algorithm>

static const char* LUA_BLOCKS_NAMESPACE = "Blocks";
static const std::string DEFAULT_BLOCK("defaultblock");

void PropertyManager::updateTickCountEvents()
{
	for (BlockProperties& blockProperties : m_blockProperties)
	{
		for (BlockEventTrigger& e : blockProperties.events)
		{
			if (e.left.type == BlockPropertyValueType::LUA_TICKCOUNTER) {
				e.left.value++;
				if (e.left.value > e.right.value)	// both always integer type
					e.left.value = 0;
			}
		}
	}
}

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

		BlockPropertyValue leftProp = getValue(left, properties.perBlockProperties);
		BlockPropertyValue rightProp = getValue(right, properties.perBlockProperties);
		
		luabridge::LuaRef eventRef = e.second["event"];
		BlockEventTrigger trigger(leftProp, rightProp, eval, eventRef);

#ifdef _DEBUG
		assert(rightProp.type != LUA_TICKCOUNTER && "Only left side of trigger may be a tickCount");	//TODO: fix?
		if (leftProp.type == LUA_TICKCOUNTER)
		{
			assert(eval == EQUAL && "tickCount may only use == expression");
			assert(rightProp.type == LUA_INT && "tickCount may only be compared with an integer type");
		}
#endif //_DEBUG

		properties.events.push_back(trigger);
		printf("trigger: %i, %i, %i \n", trigger.left.type, trigger.right.type, trigger.eval);
	}
}

BlockPropertyValue PropertyManager::getValue(std::string str, const std::vector<PerBlockProperty>& perBlockProperties)
{
	for (unsigned int i = 0; i < perBlockProperties.size(); ++i)
	{
		const PerBlockProperty& p = perBlockProperties[i];
		if (str == p.name)
			return BlockPropertyValue(i, LUA_PROPERTY_REF);
	}

	if (str == "true")
		return BlockPropertyValue(1, LUA_BOOL);
	if (str == "false")
		return BlockPropertyValue(0, LUA_BOOL);

	if (str == LUA_TICK_COUNT_NAME)
		return BlockPropertyValue(0, LUA_TICKCOUNTER);
	if (str == "x")
		return BlockPropertyValue(0, LUA_BLOCK_X);
	if (str == "y")
		return BlockPropertyValue(0, LUA_BLOCK_Y);
	if (str == "z")
		return BlockPropertyValue(0, LUA_BLOCK_Z);

	// check if contains a decimal point
	auto it = str.find('.');
	if (it != std::string::npos)
	{
		float f = (float) atof(str.c_str());
		int floatBits = *(int*) &f;
		return BlockPropertyValue(floatBits, LUA_FLOAT);
	}
	else
	{
		int i = atoi(str.c_str());
		return BlockPropertyValue(i, LUA_INT);
	}
}

void PropertyManager::parsePerBlockProperties(BlockProperties& properties)
{
	luabridge::LuaRef perBlock = properties.luaRef["perBlock"];

	LuaTableData& perBlockLuaProperties = getTableData(perBlock);

	properties.perBlockProperties.reserve(perBlockLuaProperties.data.size());
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

		properties.perBlockProperties.push_back(PerBlockProperty(key, r.second, type, defaultVal));
	}
}