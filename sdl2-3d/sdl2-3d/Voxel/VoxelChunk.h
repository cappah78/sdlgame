#ifndef VOXEL_CHUNK_H_
#define VOXEL_CHUNK_H_

#include <assert.h>
#include <glm\glm.hpp>

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
private:
	/** Container for abitrary size/amount per-block data */
	class ChunkDataContainer
	{
	public:
		ChunkDataContainer(unsigned int initialSize = 0);
		~ChunkDataContainer();

		std::vector<unsigned int> m_blockDataPositions;

		/** copy the data to the list, returning its start index */
		unsigned int add(void* data, unsigned int size);
		void* get(unsigned int position, unsigned int size);
		/** invalidates all returned positions*/
		void remove(unsigned int position, unsigned int size);
		unsigned int getSize() { return m_size; };

	private:
		void* m_dataBegin;
		unsigned int m_size;
		unsigned int m_used;

		void shiftPositionData(unsigned int position, unsigned int amount);
		void resize(unsigned int newSize);
	};

public:
	VoxelChunk(PropertyManager& propertyManager, const glm::ivec3& blockPos);
	VoxelChunk(const VoxelChunk& copyMe) = delete;
	~VoxelChunk() {};

	/** If any rendering related things have not yet been updated by the renderer, this is false */
	bool m_updated;
	/** Position in chunks, block pos is * CHUNK_SIZE this*/
	const glm::ivec3 m_pos;

	/** Set the block at the given position inside this chunk (0-chunksize) with optional additional data for this block */
	void setBlock(BlockID blockID, const glm::ivec3& blockPos, void* dataPtr = NULL, unsigned int dataSize = 0);
	/** Set the color at the given position inside this chunk (0-chunksize) */
	void setBlockColor(const glm::ivec3& blockPos, BlockColor color);

	/** Get the block at the given position inside this chunk (0-chunksize) */
	BlockID getBlockID(const glm::ivec3& blockPos) const;
	/** Get the color at the given position inside this chunk (0-chunksize) */
	BlockColor getBlockColor(const glm::ivec3& blockPos) const;

	/** get the color at the index in the color array, no bounds checking is done */
	inline BlockColor getBlockColor(unsigned int idx) const
	{
		return m_blockColors[idx];
	};
	/** get the blockID at the index in the color array, no bounds checking is done */
	inline BlockID getBlockID(unsigned int idx) const
	{
		return m_blockIDs[idx];
	};
	/** get the solidity at the index in the color array, no bounds checking is done */
	inline bool getSolid(unsigned int idx) const
	{
		return m_solid[idx];
	}
	/** get the sky visibility at the index in the color array, no bounds checking is done */
	inline bool getSkyVisible(unsigned int idx) const
	{
		return m_skyVisible[idx];
	};

	inline static unsigned int getBlockIndex(const glm::ivec3& blockPos)
	{
		assert(blockPos.x >= 0 && blockPos.x < CHUNK_SIZE);
		assert(blockPos.y >= 0 && blockPos.y < CHUNK_SIZE);
		assert(blockPos.z >= 0 && blockPos.z < CHUNK_SIZE);
		unsigned int idx = blockPos.x * CHUNK_SIZE_SQUARED + blockPos.y * CHUNK_SIZE + blockPos.z;
		return idx;
	}

private:
	std::vector<BlockColor> m_blockColors;
	std::vector<BlockID> m_blockIDs;
	std::vector <bool> m_solid;
	std::vector<bool> m_skyVisible;

	ChunkDataContainer m_data;
	PropertyManager& m_propertyManager;
};

#endif //VOXEL_CHUNK_H_