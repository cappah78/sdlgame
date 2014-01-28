#include "VoxelChunk.h"

#include <LuaBridge.h>
#include <lua.h>

static const Color8888 DEFAULT_LIGHT_LEVEL(0, 0, 0, 255);
static const Color8888 SOLID_BLOCK_COLOR(0, 0, 0, 0);

VoxelChunk::VoxelChunk(PropertyManager& propertyManager, const glm::ivec3& pos)
	: m_pos(pos)
	, m_propertyManager(propertyManager)
	, m_perBlockData(0)
	, m_shouldUpdate(false)
{
	m_blocks.resize(CHUNK_SIZE_CUBED);
}

void VoxelChunk::setBlock(BlockID blockID, const glm::ivec3& blockPos, void* dataPtr, unsigned int dataSize)
{
	m_shouldUpdate = true;

	VoxelBlock& oldBlock = m_blocks[getBlockIndex(blockPos)];
	removeBlockData(oldBlock);

	if (blockID == 0)	//if air block return;
		return;

	VoxelBlock& newBlock = oldBlock;	//just for verbosity
	newBlock.id = blockID;
	newBlock.solid = blockID != 0; //all non air blocks are solid for now.
	setDefaultBlockData(newBlock);
}

void VoxelChunk::setDefaultBlockData(VoxelBlock& block)
{
	const BlockProperties& blockProperties = m_propertyManager.getBlockProperties(block.id);
	const std::vector<PerBlockProperty>& propertyList = blockProperties.perBlockProperties;
	if (propertyList.size() != 0)	// if this block should have perblock values
	{
		std::vector<int> dataList;	//properties are just a list of integer values for now
		dataList.reserve(propertyList.size());
		for (const PerBlockProperty& p : propertyList)
		{
			dataList.push_back(p.defaultValue.valueBits);
		}
		block.blockDataIndex = m_perBlockData.add(&dataList[0], sizeof(int) * dataList.size());
	}
	else
	{
		block.blockDataIndex = NO_BLOCK_DATA;
	}
}

void VoxelChunk::removeBlockData(VoxelBlock& block)
{
	if (block.id == 0)	//if air block return.
		return;

	const BlockProperties& properties = m_propertyManager.getBlockProperties(block.id);
	const std::vector<PerBlockProperty>& oldPerBlockProperties = properties.perBlockProperties;

	// each property is an int for now
	unsigned int oldBlockPropertiesSize = oldPerBlockProperties.size() * sizeof(int);

	//There was already a block here with per block data, clean up data
	if (block.blockDataIndex != NO_BLOCK_DATA && oldBlockPropertiesSize != 0) 
	{
		m_perBlockData.remove(block.blockDataIndex, oldBlockPropertiesSize);
		// shift all the elements after data index by size to the left.
		for (VoxelBlock& block : m_blocks)
		{
			if (block.blockDataIndex > block.blockDataIndex )
				block.blockDataIndex -= oldBlockPropertiesSize;
		}
	}
}

void VoxelChunk::updateBlockEventTriggers(VoxelBlock& block, const glm::ivec3& blockWorldPos)
{
	if (block.id == 0)	//if block is air, skip
		return;

	const BlockProperties& properties = m_propertyManager.getBlockProperties(block.id);
	for (BlockEventTrigger e : properties.events)
	{
		bool triggered = false;
		switch (e.eval)
		{
		case EventEvaluator::EQUAL:
			triggered = (e.left == e.right);
			break;
		case EventEvaluator::GREATER:
			triggered = (e.left > e.right);
			break;
		case EventEvaluator::GREATEREQUALS:
			triggered = (e.left >= e.right);
			break;
		case EventEvaluator::LESS:
			triggered = (e.left < e.right);
			break;
		case EventEvaluator::LESSEQUALS:
			triggered = (e.left <= e.right);
			break;
		}
		if (triggered)
		{
			// create a lua table for the blockUpdate method argument
			luabridge::LuaRef luaBlockArg = luabridge::newTable(properties.luaRef.state());

			luaBlockArg["x"] = blockWorldPos.x;	//add useful values
			luaBlockArg["y"] = blockWorldPos.y;
			luaBlockArg["z"] = blockWorldPos.z;

			BlockID oldID = block.id;
			bool hasProperties = properties.perBlockProperties.size() > 0;
			if (hasProperties)
				insertBlockData(block, luaBlockArg);
			e.process(luaBlockArg);
			// update data with changes made in script, only if the block was not removed
			if (hasProperties && block.id == oldID)
				updateBlockData(block, luaBlockArg);
		}
	}
}

void VoxelChunk::insertBlockData(const VoxelBlock& block, luabridge::LuaRef& luaBlockArg)
{
	int* dataList = getPerBlockDataList(block.blockDataIndex);
	const BlockProperties& properties = m_propertyManager.getBlockProperties(block.id);
	const std::vector<PerBlockProperty>& propertyList = properties.perBlockProperties;
	for (unsigned int i = 0; i < propertyList.size(); ++i)
	{ // iterate the data, put property value into table under property name
		const PerBlockProperty& p = propertyList[i];
		float f;
		switch (p.defaultValue.type)
		{
		case BlockPropertyValueType::LUA_INT:
			luaBlockArg[p.name] = dataList[i];
			break;
		case BlockPropertyValueType::LUA_FLOAT:
			f = *(float*) dataList[i];	//cast bits to float
			luaBlockArg[p.name] = f;
			break;
		case BlockPropertyValueType::LUA_BOOL:
			luaBlockArg[p.name] = dataList[i] == 1;
			break;
		}
	}
}

void VoxelChunk::updateBlockData(const VoxelBlock& block, luabridge::LuaRef& table)
{
	int* dataList = getPerBlockDataList(block.blockDataIndex);
	const BlockProperties& properties = m_propertyManager.getBlockProperties(block.id);
	const std::vector<PerBlockProperty>& propertyList = properties.perBlockProperties;

	for (unsigned int i = 0; i < propertyList.size(); ++i)
	{
		const PerBlockProperty& p = propertyList[i];
		float f;
		switch (p.defaultValue.type)
		{
		case BlockPropertyValueType::LUA_INT:
			dataList[i] = table[p.name];
			break;
		case BlockPropertyValueType::LUA_FLOAT:
			f = table[p.name];
			dataList[i] = *(int*) &f;	//cast float to int bits
			break;
		case BlockPropertyValueType::LUA_BOOL:
			dataList[i] = table[p.name] ? 1 : 0;
			break;
		default:
			assert(false);
			break;
		}
	}
}

void VoxelChunk::doBlockUpdate(const glm::ivec3& blockChunkPos, const glm::ivec3& blockWorldPos)
{

	const VoxelBlock& block = getBlock(getBlockIndex(blockChunkPos));
	const BlockProperties& properties = m_propertyManager.getBlockProperties(block.id);

	printf("update: %i : %i : %i : %i \n", blockWorldPos.x, blockWorldPos.y, blockWorldPos.z, block.id);


	if (!properties.hasBlockUpdateMethod)
		return;

	// create a lua table for the blockUpdate method argument
	luabridge::LuaRef luaBlockArg = luabridge::newTable(properties.luaRef.state());

	luaBlockArg["x"] = blockWorldPos.x;	//add useful values
	luaBlockArg["y"] = blockWorldPos.y;
	luaBlockArg["z"] = blockWorldPos.z;

	BlockID oldID = block.id;
	bool hasProperties = properties.perBlockProperties.size() > 0;
	if (hasProperties)
		insertBlockData(block, luaBlockArg);
	properties.blockUpdateMethod(luaBlockArg);
	// update data with changes made in script, only if the block was not removed
	if (hasProperties && block.id == oldID)
		updateBlockData(block, luaBlockArg);
}
