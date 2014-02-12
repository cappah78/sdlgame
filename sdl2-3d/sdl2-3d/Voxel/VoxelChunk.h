#pragma once

#include "../Voxel/VoxelBlock.h"
#include "../Engine/Utils/VoidDataList.h"

#include <glm\glm.hpp>
#include <vector>

class PropertyManager;
class WorldRenderer;

/** Chunk size in one dimension */
static const unsigned int CHUNK_SIZE = 16;
static const unsigned int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
static const unsigned int CHUNK_SIZE_CUBED = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

class VoxelChunk
{
public:
	VoxelChunk(PropertyManager& propertyManager, const glm::ivec3& chunkPos);
	//VoxelChunk(const VoxelChunk& copyMe) = delete;
	~VoxelChunk() {};

	void doBlockUpdate(const glm::ivec3& blockChunkPos, const glm::ivec3& blockWorldPos);

	/** Set the block at the given position inside this chunk (0-chunksize) with optional additional data for this block */
	void setBlock(BlockID blockID, const glm::ivec3& blockPos, void* dataPtr = NULL, unsigned int dataSize = 0);

	/** Get the block at the given position inside this chunk (0-chunksize) */
	const VoxelBlock& getBlock(const glm::ivec3& blockPos) const
	{
		return getBlock(getBlockIndex(blockPos));
	}
	const BlockProperties& getBlockProperties(BlockID blockID) const;

	inline int* getPerBlockDataList(unsigned int blockDataIndex) const
	{
		return static_cast<int*>(m_perBlockData.get(blockDataIndex));
	}

	/** get the color at the index in the color array, no bounds checking is done */
	inline const VoxelBlock& getBlock(unsigned int idx) const
	{
		return m_blocks[idx];
	};

	inline static unsigned int getBlockIndex(const glm::ivec3& blockPos)
	{
		assert(blockPos.x >= 0 && blockPos.x < CHUNK_SIZE);
		assert(blockPos.y >= 0 && blockPos.y < CHUNK_SIZE);
		assert(blockPos.z >= 0 && blockPos.z < CHUNK_SIZE);
		unsigned int idx = blockPos.x * CHUNK_SIZE_SQUARED + blockPos.y * CHUNK_SIZE + blockPos.z;
		return idx;
	}

	/** Get the total amount of per block data this chunk contains in bytes*/
	inline unsigned int getPerBlockDataListSize()
	{
		return m_perBlockData.getSize();
	};

	void updateBounds();

	/** If any rendering related things have not yet been updated by the renderer, this is false */
	bool m_shouldUpdate;
	/** Position in chunks, block pos is * CHUNK_SIZE this*/
	const glm::ivec3 m_pos;

private:
	static const int NO_BLOCK_DATA = -1;

	/** Insert all the properties of the given block into the table */
	void insertBlockData(const VoxelBlock& block, luabridge::LuaRef& table);
	/** Read and update all properties of the given block using the table's data */
	void updateBlockData(const VoxelBlock& block, luabridge::LuaRef& table);

	void removeBlockData(VoxelBlock& block);
	void setDefaultBlockData(VoxelBlock& block);

	void updateBlockEventTriggers(VoxelBlock& block, const glm::ivec3& blockWorldPos);
	void doBlockProcess(VoxelBlock& block, const glm::ivec3& blockWorldPos);

	VoidDataList m_perBlockData;
	std::vector<VoxelBlock> m_blocks;

	PropertyManager& m_propertyManager;
};