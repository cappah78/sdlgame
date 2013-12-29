#ifndef VOXEL_CHUNK_H_
#define VOXEL_CHUNK_H_

#include <glm\glm.hpp>

#include "VoxelBlock.h"

/** Chunk size in one dimension */
static const unsigned int CHUNK_SIZE = 16;
static const unsigned int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
static const unsigned int CHUNK_SIZE_CUBED = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

#include <vector>

class PropertyManager;

class VoxelChunk
{
private:
	/** Container for any amount of random per-block data*/
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

	bool m_updated;
	const glm::ivec3 m_pos;

	/** Set the block at the given position inside this chunk (0-chunksize) with optional additional data for this block */
	void setBlock(BlockID blockID, const glm::ivec3& blockPos, void* dataPtr = NULL, unsigned int dataSize = 0);
	/** Set the color at the given position inside this chunk (0-chunksize) */
	void setBlockColor(const glm::ivec3& blockPos, BlockColor color);

	/** Get the block at the given position inside this chunk (0-chunksize) */
	BlockID getBlockID(const glm::ivec3& blockPos) const;
	/** Return the backing array of id's, index = x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z */
	BlockID* getBlocks() { return &m_blockIDs[0]; };

	/** Get the color at the given position inside this chunk (0-chunksize) */
	BlockColor getBlockColor(const glm::ivec3& blockPos) const;
	/** Return the backing array of colors, index = x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z */
	BlockColor* getColors() { return &m_blockColors[0]; };

	BlockIDColor getBlockIDColor(const glm::ivec3& pos) const;

private:

	ChunkDataContainer m_data;
	PropertyManager& m_propertyManager;

	std::vector<BlockColor> m_blockColors;
	std::vector<BlockID> m_blockIDs;
	std::vector<bool> m_skyVisible;
};

#endif //VOXEL_CHUNK_H_