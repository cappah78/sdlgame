#include "VoxelChunk.h"

#include "PropertyManager.h"
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
	m_blockDataPositions.resize(CHUNK_SIZE_CUBED, -1);
	m_blockColors.resize(CHUNK_SIZE_CUBED, DEFAULT_LIGHT_LEVEL);
	m_blockIDs.resize(CHUNK_SIZE_CUBED, 0);
	m_skyVisible.resize(CHUNK_SIZE_CUBED, false);
	m_solid.resize(CHUNK_SIZE_CUBED, false);
}

void VoxelChunk::setBlock(BlockID blockID, const glm::ivec3& blockPos, void* dataPtr, unsigned int dataSize)
{
	unsigned int idx = getBlockIndex(blockPos);

	m_updated = false;

	BlockID& oldID = m_blockIDs[idx];
	int& oldDataPos = m_blockDataPositions[idx];
	m_solid[idx] = m_propertyManager.isSolid(blockID);

	if (oldID != 0) //if there was already a block here
	{
		//handle on block remove
		//TODO: call block remove lua stuff
		if (oldDataPos != NO_BLOCK_DATA && m_propertyManager.hasPerBlockProperties(oldID)) //There was already a block here with per block data, clean up data
		{
			unsigned int oldSize = m_propertyManager.getPerBlockProperties(oldID).sizeBytes;
			m_perBlockData.remove(oldDataPos, oldSize);
			shiftPositionIndices(oldDataPos, oldSize);
		}
	}

	if (m_propertyManager.hasPerBlockProperties(blockID))	// if this block should have perblock values
	{
		PerBlockProperties& properties = m_propertyManager.getPerBlockProperties(blockID);

		if (dataSize != 0 && dataPtr != NULL)	//if given initial values for this block
		{
			assert(dataSize == properties.sizeBytes && "Given data size not equal to block properties");
			int dataPos = m_perBlockData.add(dataPtr, dataSize);
			oldDataPos = dataPos;
		}
		else //insert default values
		{
			std::vector<int> dataList;
			dataList.reserve(properties.sizeBytes / 4);
			for (PerBlockProperty& p : properties.properties)
			{				
				dataList.push_back(p.defaultValue);
			}
			unsigned int dataPos = m_perBlockData.add(&dataList[0], sizeof(int) * dataList.size());
			oldDataPos = dataPos;
		}
	}
	else
	{
		assert(dataSize == 0 && dataPtr == 0);	//if no per block data, check that none is given.
		oldDataPos = NO_BLOCK_DATA;
	}

	oldID = blockID;
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
				int idx = getBlockIndex(glm::ivec3(x, y, z));

				BlockID id = m_blockIDs[idx];

				if (id == 0)	//if block is air, skip
					continue;

				luabridge::LuaRef block = m_propertyManager.getLuaBlockRef(id);
				luabridge::LuaRef process = block["process"];
				if (process.isNil())	//if block has no process function, skip.
					continue;

				luabridge::LuaRef luaBlockArg = luabridge::newTable(block.state());	// create a lua table for the process argument
				luaBlockArg["x"] = x + chunkOffset.x;	// add useful things.
				luaBlockArg["y"] = y + chunkOffset.y;
				luaBlockArg["z"] = z + chunkOffset.z;

				int blockDataPos = m_blockDataPositions[idx];
				if (blockDataPos != NO_BLOCK_DATA)	//if block has per block data
				{
					PerBlockProperties& properties = m_propertyManager.getPerBlockProperties(id);// get the properties of the data

					void* data = m_perBlockData.get(blockDataPos);	//get the data
					int* val = static_cast<int*>(data);	// iterate the data, put property value into table under property name
					for (PerBlockProperty& p : properties.properties)
					{
						float f;
						switch (p.type)
						{
						case BlockPropertyValueType::LUA_INT:
							luaBlockArg[p.name] = *val;
							break;
						case BlockPropertyValueType::LUA_FLOAT:
							f = *(float*) val;	//cast bits to float
							luaBlockArg[p.name] = f;
							break;
						case BlockPropertyValueType::LUA_BOOL:
							luaBlockArg[p.name] = (*val) == 1;
							break;
						}
						val++;	//increment pointer by 4 bytes;
					}
				}

				try
				{
					process(luaBlockArg);	//run Blocks.blocktype.process
				}
				catch (luabridge::LuaException const& e)
				{
					printf("%s \n", e.what());
				}

				int newDataPos = m_blockDataPositions[idx];
				if (newDataPos != NO_BLOCK_DATA)	//check to see if the block still exists (might be removed during process)
				{
					PerBlockProperties& properties = m_propertyManager.getPerBlockProperties(id);// get the properties of the data

					void* newData = m_perBlockData.get(blockDataPos); //get the data
					int* val = static_cast<int*>(newData); // iterate the data, updating it with the values from the lua table given as argument to process.
					for (PerBlockProperty& p : properties.properties)
					{
						float f;
						switch (p.type)
						{
						case BlockPropertyValueType::LUA_INT:
							*val = luaBlockArg[p.name];
							break;
						case BlockPropertyValueType::LUA_FLOAT:
							f = luaBlockArg[p.name];
							*val = *(int*) &f;	//cast float to int bits
							break;
						case BlockPropertyValueType::LUA_BOOL:
							if (luaBlockArg[p.name])
								*val = 1;
							else
								*val = 0;
							break;
						}
						val++;	//increment pointer by 4 bytes;
					}
				}
			}
		}
	}
}

void VoxelChunk::setBlockColor(const glm::ivec3& blockPos, BlockColor color)
{
	m_updated = false;

	unsigned int idx = getBlockIndex(blockPos);
	m_blockColors[idx] = color;
}

BlockID VoxelChunk::getBlockID(const glm::ivec3& blockPos) const
{
	unsigned int idx = getBlockIndex(blockPos);
	return m_blockIDs[idx];
}

BlockColor VoxelChunk::getBlockColor(const glm::ivec3& blockPos) const
{
	unsigned int idx = getBlockIndex(blockPos);
	return m_blockColors[idx];
}

void VoxelChunk::shiftPositionIndices(int position, unsigned int amount)
{
	// shift all the elements after position by amount to the left.
	for (int& blockDataPos : m_blockDataPositions)
	{
		if (blockDataPos > position)
			blockDataPos -= amount;
	}
}

