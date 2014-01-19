#include "VoxelChunk.h"

#include <LuaBridge.h>
#include <lua.h>

static const Color8888 DEFAULT_LIGHT_LEVEL(0, 0, 0, 255);
static const Color8888 SOLID_BLOCK_COLOR(0, 0, 0, 0);

VoxelChunk::VoxelChunk(PropertyManager& propertyManager, const glm::ivec3& pos)
	: m_pos(pos)
	, m_propertyManager(propertyManager)
	, m_perBlockData(0)
	, m_updated(false)
{
	updateBounds();
	m_blocks.resize(CHUNK_SIZE_CUBED);
}

void VoxelChunk::updateBounds()
{
	m_bounds[0] = glm::vec3(m_pos.x * (float) CHUNK_SIZE, m_pos.y * (float) CHUNK_SIZE, m_pos.z * (float) CHUNK_SIZE);
	m_bounds[1] = glm::vec3(m_bounds[0] + glm::vec3(CHUNK_SIZE, 0, 0));
	m_bounds[2] = glm::vec3(m_bounds[0] + glm::vec3(CHUNK_SIZE, CHUNK_SIZE, 0));
	m_bounds[3] = glm::vec3(m_bounds[0] + glm::vec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE));
	m_bounds[4] = glm::vec3(m_bounds[0] + glm::vec3(0, CHUNK_SIZE, 0));
	m_bounds[5] = glm::vec3(m_bounds[0] + glm::vec3(0, CHUNK_SIZE, CHUNK_SIZE));
	m_bounds[6] = glm::vec3(m_bounds[0] + glm::vec3(0, 0, CHUNK_SIZE));
	m_bounds[7] = glm::vec3(m_bounds[0] + glm::vec3(CHUNK_SIZE, 0, CHUNK_SIZE));
}

void VoxelChunk::setBlock(BlockID blockID, const glm::ivec3& blockPos, void* dataPtr, unsigned int dataSize)
{
	unsigned int idx = getBlockIndex(blockPos);
	VoxelBlock& block = m_blocks[idx];
	m_updated = false;

	if (block.id != 0) //if there was already a block here
	{
		const BlockProperties& oldProperties = m_propertyManager.getBlockProperties(block.id);
		const std::vector<PerBlockProperty>& oldPerBlockProperties = oldProperties.perBlockProperties;

		unsigned int oldBlockPropertiesSize = oldPerBlockProperties.size() * sizeof(int);	// each property is an int
		if (block.blockDataIndex != NO_BLOCK_DATA && oldBlockPropertiesSize != 0) //There was already a block here with per block data, clean up data
		{
			m_perBlockData.remove(block.blockDataIndex, oldBlockPropertiesSize);	//remove per block data of this block.
			shiftPositionIndices(block.blockDataIndex, oldBlockPropertiesSize);		//required after remove. TODO: clean
		}
	}

	if (blockID != 0)
	{
		const BlockProperties& newProperties = m_propertyManager.getBlockProperties(blockID);
		const std::vector<PerBlockProperty>& newPerBlockProperties = newProperties.perBlockProperties;

		if (newPerBlockProperties.size() != 0)	// if this block should have perblock values
		{
			if (dataSize != 0 && dataPtr != NULL)	//if given initial values for this block
			{
				block.blockDataIndex = m_perBlockData.add(dataPtr, dataSize);
			}
			else //insert default values
			{
				std::vector<int> dataList;	//properties are just a list of integer values for now
				dataList.reserve(newPerBlockProperties.size());
				for (const PerBlockProperty& p : newPerBlockProperties)
				{
					dataList.push_back(p.prop.value);
				}
				unsigned int dataPos = m_perBlockData.add(&dataList[0], sizeof(int) * dataList.size());
				block.blockDataIndex = dataPos;
			}
		}
		else
		{
			assert(dataSize == 0 && dataPtr == 0);	//if no per block data, check that none is given.
			block.blockDataIndex = NO_BLOCK_DATA;
		}
	}

	block.id = blockID;
	block.update = true;
	block.solid = blockID != 0; //all non air blocks are solid for now.
}

void VoxelChunk::doBlockEvents(const glm::ivec3& blockPos, const glm::ivec3& chunkOffset)
{
	int idx = getBlockIndex(blockPos);
	VoxelBlock& block = m_blocks[idx];

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

	}
}

/** Set the values in the given lua table, to match the data in the given array using the properties */
void setProperties(int* dataArr, const std::vector<PerBlockProperty>& propertyList, luabridge::LuaRef& luaBlockArg)
{
	for (unsigned int i = 0; i < propertyList.size(); ++i)
	{ // iterate the data, put property value into table under property name
		const PerBlockProperty& p = propertyList[i];
		float f;
		switch (p.prop.type)
		{
		case BlockPropertyValueType::LUA_INT:
			luaBlockArg[p.name] = dataArr[i];
			break;
		case BlockPropertyValueType::LUA_FLOAT:
			f = *(float*) dataArr[i];	//cast bits to float
			luaBlockArg[p.name] = f;
			break;
		case BlockPropertyValueType::LUA_BOOL:
			luaBlockArg[p.name] = dataArr[i] == 1;
			break;
		}
	}
}

void updateProperties(int* dataArr, const std::vector<PerBlockProperty>& propertyList, luabridge::LuaRef& luaBlockArg)
{
	for (unsigned int i = 0; i < propertyList.size(); ++i)
	{
		const PerBlockProperty& p = propertyList[i];
		float f;
		switch (p.prop.type)
		{
		case BlockPropertyValueType::LUA_INT:
			dataArr[i] = luaBlockArg[p.name];
			break;
		case BlockPropertyValueType::LUA_FLOAT:
			f = luaBlockArg[p.name];
			dataArr[i] = *(int*) &f;	//cast float to int bits
			break;
		case BlockPropertyValueType::LUA_BOOL:
			if (luaBlockArg[p.name])
				dataArr[i] = 1;
			else
				dataArr[i] = 0;
			break;
		default:
			assert(false);
			break;
		}
	}
}

void VoxelChunk::doBlockProcess(const glm::ivec3& blockPos, const glm::ivec3& chunkOffset)
{
	int idx = getBlockIndex(blockPos);
	VoxelBlock& block = m_blocks[idx];

	if (block.id == 0)	//if block is air, skip
		return;

	int startDataIdx = block.blockDataIndex;

	const BlockProperties& properties = m_propertyManager.getBlockProperties(block.id);
	const std::vector<PerBlockProperty>& perBlockProperties = properties.perBlockProperties;
	luabridge::LuaRef process = properties.luaRef["process"];

	if (properties.events.size() == 0)	// if block has no no events to check, skip.
		return;

	// if block has per block properties, get the list, else null.
	int* dataList = perBlockProperties.size() != 0 ? getPerBlockDataList(block.blockDataIndex) : NULL;

	luabridge::LuaRef luaBlockArg = luabridge::newTable(properties.luaRef.state());	// create a lua table for the process argument
	glm::ivec3 blockWorldPos = blockPos + chunkOffset;

	luaBlockArg["x"] = blockWorldPos.x;	//add useful values
	luaBlockArg["y"] = blockWorldPos.y;
	luaBlockArg["z"] = blockWorldPos.z;

	bool setPerBlockProperties = false;

	for (BlockEventTrigger e : properties.events)	//by value because we dont want to change the default
	{	

		// update block values before evaluation.
		switch (e.left.type)
		{
		case BlockPropertyValueType::LUA_BLOCK_X:
			e.left.value = blockWorldPos.x;
			break;
		case BlockPropertyValueType::LUA_BLOCK_Y:
			e.left.value = blockWorldPos.y;
			break;
		case BlockPropertyValueType::LUA_BLOCK_Z:
			e.left.value = blockWorldPos.z;
			break;
		case BlockPropertyValueType::LUA_PROPERTY_REF:	// if this type, the value will be the index of the property
			assert(dataList != 0 && "Block does not have per block properties");
			// get the type from the properties, and the value from the data list
			e.left.type = properties.perBlockProperties.at(e.left.value).prop.type;
			e.left.value = dataList[e.left.value];
			break;
		}

		bool triggered = e.isTriggered();
		
		if (triggered)
		{
			if (!setPerBlockProperties && perBlockProperties.size() != 0)	// fill lua table argument with the properties(if any), only needs to be done once.
			{
				setPerBlockProperties = true;
				setProperties(dataList, perBlockProperties, luaBlockArg);
			}
			e.process(luaBlockArg);
		}
	}

	luabridge::LuaRef onBlockUpdate = properties.luaRef["onBlockUpdate"];
	if (!onBlockUpdate.isNil() && block.update)	// run onBlockupdate if block.update is true, might move to its own loop.
	{
		block.update = false;
		onBlockUpdate(luaBlockArg);
	}

	// if a process happened, and there are per block properties, update the dataList to reflect the changes, only if the block was not removed
	if (setPerBlockProperties && block.blockDataIndex == startDataIdx)
	{
		updateProperties(dataList, perBlockProperties, luaBlockArg);
	}
}

void VoxelChunk::doBlockUpdate()
{
	glm::ivec3& chunkOffset = m_pos * (int)CHUNK_SIZE;

	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
			{
				glm::ivec3 blockPos(x, y, z);

				doBlockProcess(blockPos, chunkOffset);
			}
		}
	}
}

void VoxelChunk::shiftPositionIndices(int position, unsigned int amount)
{
	// shift all the elements after position by amount to the left.
	for (VoxelBlock& block : m_blocks)
	{
		if (block.blockDataIndex > position)
			block.blockDataIndex -= amount;
	}
}

