#ifndef PROPERTY_MANAGER_H_
#define PROPERTY_MANAGER_H_

#include "VoxelBlock.h"
#include "TextureManager.h"

#include <assert.h>
#include <map>
#include <set>
#include <vector>
#include <memory>

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
	PropertyManager()
		: m_lastRegisteredId(0) 

	{
	};
	PropertyManager(const PropertyManager& copy) = delete;
	~PropertyManager() {};

	BlockID registerBlockType(lua_State* const L, const std::string& blockname);

	void updateTickCountEvents();

	unsigned short getBlockTextureID(const std::string& textureName);
	std::shared_ptr<TextureArray> generateBlockTextureArray(unsigned int blockTexWidth, unsigned int blockTexHeight);

	inline BlockID getBlockID(const std::string& blockName)	const		{ return m_blockNameIDMap.at(blockName); };
	inline const BlockProperties& getBlockProperties(BlockID blockID) const	{ return m_blockProperties[blockID]; }
	inline BlockID getNumRegisteredBlocks() const { return m_lastRegisteredId; };
private:

	LuaTableData getTableData(luabridge::LuaRef ref) const;
	void parseBlock(BlockProperties& properties);
	void parseEvents(BlockProperties& properties);
	BlockPropertyValue getValue(std::string str, const std::vector<PerBlockProperty>& perBlockProperties);
	void parsePerBlockProperties(BlockProperties& properties);

	/** ID counter used to give every registered block an unique id */
	BlockID m_lastRegisteredId;
	/** Maps Block name to its id */
	std::map<const std::string, BlockID> m_blockNameIDMap;

	/** idx == blockID, value luarefs to access the lua values of this block */
	std::vector<luabridge::LuaRef> m_luaBlockRefs;

	/** Idx == blockID, value.left = tick counter (incrementing), value.right = num ticks required for process(constant) */
	std::vector<std::vector<BlockEventTrigger>> m_tickCountEvents;

	std::vector<BlockProperties> m_blockProperties;

	std::map<std::string, unsigned short> m_blockTextureIDNames;
	std::vector<std::string> m_blockTextureNames;
};

#endif // PROPERTY_MANAGER_H_