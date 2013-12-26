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
public:
	VoxelChunk(PropertyManager& propertyManager, const glm::ivec3& pos)
		: m_pos(pos)
		, m_propertyManager(propertyManager)
		, m_data(0)
		, m_updated(false)
	{};
	VoxelChunk(const VoxelChunk& copyMe) = delete;
	~VoxelChunk() {};

	bool m_updated;

	void setBlock(BlockID blockID, int x, int y, int z, void* dataPtr = NULL, unsigned int dataSize = 0);
	BlockID getBlockID(int x, int y, int z);
	BlockID* getBlocks() { return &m_data.m_blockIDs[0]; };

	const glm::ivec3 m_pos;
private:

	class ChunkDataContainer
	{
	public:
		std::vector<BlockID> m_blockIDs;
		std::vector<unsigned int> m_blockDataPositions;

		ChunkDataContainer(unsigned int initialSize = 0);
		~ChunkDataContainer();
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

	ChunkDataContainer m_data;
	PropertyManager& m_propertyManager;
};

#endif //VOXEL_CHUNK_H_