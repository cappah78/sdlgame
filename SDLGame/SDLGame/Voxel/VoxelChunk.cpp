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
	m_blocks.resize(CHUNK_SIZE_CUBED);
}

void VoxelChunk::setBlock(BlockID blockID, const glm::ivec3& blockPos, void* dataPtr, unsigned int dataSize)
{
	unsigned int idx = getBlockIndex(blockPos);
	VoxelBlock& block = m_blocks[idx];
	m_updated = false;

	if (block.id != 0) //if there was already a block here
	{
		//handle on block remove
		//TODO: call block remove lua stuffa
		unsigned int oldBlockPropertiesSize = m_propertyManager.getBlockProperties(block.id).propertiesSizeBytes;
		if (block.blockDataIndex != NO_BLOCK_DATA && oldBlockPropertiesSize != 0) //There was already a block here with per block data, clean up data
		{
			m_perBlockData.remove(block.blockDataIndex, oldBlockPropertiesSize);
			shiftPositionIndices(block.blockDataIndex, oldBlockPropertiesSize);
		}
	}

	const BlockProperties& properties = m_propertyManager.getBlockProperties(blockID);

	if (properties.propertiesSizeBytes != 0)	// if this block should have perblock values
	{
		if (dataSize != 0 && dataPtr != NULL)	//if given initial values for this block
		{
			assert(dataSize == properties.propertiesSizeBytes && "Given data size not equal to block properties");
			unsigned int dataPos = m_perBlockData.add(dataPtr, dataSize);
			block.blockDataIndex = dataPos;
		}
		else //insert default values
		{
			std::vector<int> dataList;	//properties are just a list of integer values for now
			dataList.reserve(properties.propertiesSizeBytes / 4);
			for (const PerBlockProperty& p : properties.properties)
			{				
				dataList.push_back(p.defaultValue);
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
	block.id = blockID;
	block.solid = blockID != 0; //all non air blocks are solid for now.
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

				VoxelBlock& block = m_blocks[idx];

				if (block.id == 0)	//if block is air, skip
					continue;

				const BlockProperties& properties = m_propertyManager.getBlockProperties(block.id);
				luabridge::LuaRef process = properties.luaRef["process"];
				if (process.isNil())	//if block has no process function, skip.
					continue;

				luabridge::LuaRef luaBlockArg = luabridge::newTable(properties.luaRef.state());	// create a lua table for the process argument
				luaBlockArg["x"] = x + chunkOffset.x;	// add useful things.
				luaBlockArg["y"] = y + chunkOffset.y;
				luaBlockArg["z"] = z + chunkOffset.z;

				if (properties.propertiesSizeBytes != 0)	//if block has per block data
				{
					void* data = m_perBlockData.get(block.blockDataIndex);	//get the data
					int* val = static_cast<int*>(data);	// iterate the data, put property value into table under property name
					for (const PerBlockProperty& p : properties.properties)
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
				catch (const luabridge::LuaException& e)
				{
					printf("%s \n", e.what());
				}

				if (block.blockDataIndex != NO_BLOCK_DATA)	//check to see if the block still exists (might be removed during process)
				{
					void* newData = m_perBlockData.get(block.blockDataIndex); //get the data
					int* val = static_cast<int*>(newData); // iterate the data, updating it with the values from the lua table given as argument to process.
					for (const PerBlockProperty& p : properties.properties)
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
						default:
							assert(false);
							break;
						}
						val++;	//increment pointer by 4 bytes;
					}
				}
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

