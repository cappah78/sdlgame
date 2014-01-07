#ifndef VOXEL_CHUNK_H_
#define VOXEL_CHUNK_H_

#include "PropertyManager.h"
#include "../Engine/Utils/VoidDataList.h"
#include <glm\glm.hpp>
#include <vector>

#include "VoxelBlock.h"

/** Chunk size in one dimension */
static const unsigned int CHUNK_SIZE = 16;
static const unsigned int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
static const unsigned int CHUNK_SIZE_CUBED = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

#include <vector>

class PropertyManager;
class WorldRenderer;

class VoxelChunk
{
public:
	VoxelChunk(PropertyManager& propertyManager, const glm::ivec3& blockPos);
	VoxelChunk(const VoxelChunk& copyMe) = delete;
	~VoxelChunk() {};

	void doBlockUpdate();

	/** If any rendering related things have not yet been updated by the renderer, this is false */
	bool m_updated;
	/** Position in chunks, block pos is * CHUNK_SIZE this*/
	const glm::ivec3 m_pos;

	/** Set the block at the given position inside this chunk (0-chunksize) with optional additional data for this block */
	void setBlock(BlockID blockID, const glm::ivec3& blockPos, void* dataPtr = NULL, unsigned int dataSize = 0);
	/** Set the color at the given position inside this chunk (0-chunksize) */

	/** Get the block at the given position inside this chunk (0-chunksize) */
	const VoxelBlock& getBlock(const glm::ivec3& blockPos) const
	{
		return getBlock(getBlockIndex(blockPos));
	}
	const BlockProperties& getBlockProperties(BlockID blockID) const
	{
		return m_propertyManager.getBlockProperties(blockID);
	}

	inline const void* getPerBlockDataAtIdx(unsigned int blockDataIndex) const
	{
		return m_perBlockData.get(blockDataIndex);
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

private:
	static const int NO_BLOCK_DATA = -1;

	void shiftPositionIndices(int position, unsigned int amount);

	VoidDataList m_perBlockData;
	std::vector<VoxelBlock> m_blocks;

	PropertyManager& m_propertyManager;
};

#endif //VOXEL_CHUNK_H_