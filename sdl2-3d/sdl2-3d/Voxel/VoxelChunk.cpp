#include "VoxelChunk.h"

#include <assert.h>
#include "PropertyManager.h"



VoxelChunk::ChunkDataContainer::ChunkDataContainer(unsigned int initialSize)
: m_size(initialSize)
, m_used(0)
{
	m_dataBegin = malloc(initialSize);
	assert(m_dataBegin && "Failed to allocate");
};

VoxelChunk::ChunkDataContainer::~ChunkDataContainer()
{};

/** copy the data to the list, returning its start index */
unsigned int VoxelChunk::ChunkDataContainer::add(void* data, unsigned int size)
{
	if (m_used + size > m_size)
	{
		unsigned int newSize = (unsigned int) ((m_used + size) * RESIZE_MULTIPLIER);
		resize(newSize);
	}

	unsigned int position = m_used;

	void* tail = (void*) ((char*) m_dataBegin + m_used);
	memcpy(tail, data, size);
	m_used += size;

	return position;
};

void* VoxelChunk::ChunkDataContainer::get(unsigned int position, unsigned int size)
{
	void* at = (void*) ((char*) m_dataBegin + position);
	return at;
};

/** invalidates all returned positions*/
void VoxelChunk::ChunkDataContainer::remove(unsigned int position, unsigned int size)
{
	void* removeHead = (void*) ((char*) m_dataBegin + position);
	void* removeTail = (void*) ((char*) removeHead + size);

	unsigned int moveSize = m_used - (position + size);
	memmove(removeHead, removeTail, moveSize);
	m_used -= size;

	shiftPositionData(position, size);
};

void VoxelChunk::ChunkDataContainer::shiftPositionData(unsigned int position, unsigned int amount)
{
	for (unsigned int& blockData : m_blockDataPositions)
	{
		if (blockData > position)
			blockData -= amount;
	}
};

void VoxelChunk::ChunkDataContainer::resize(unsigned int newSize)
{
	std::cout << "allocating: " << newSize << std::endl;

	void* newBegin = malloc(newSize);
	memcpy(newBegin, m_dataBegin, m_used);
	free(m_dataBegin);

	m_dataBegin = newBegin;
	m_size = newSize;
};

void VoxelChunk::setBlock(BlockID blockID, int x, int y, int z, void* dataPtr, unsigned int dataSize)
{
	assert(x >= 0 && x < CHUNK_SIZE);
	assert(y >= 0 && y < CHUNK_SIZE);
	assert(z >= 0 && z < CHUNK_SIZE);

	unsigned int idx = x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z;
	BlockID& id = m_data.m_blockIDs[idx];

	if (id != 0)//there was already a block here
	{
		//handle on block remove
	}

	unsigned int& dataPos = m_data.m_blockDataPositions[idx];
	if (dataPos != 0)//There was already a block here with data
	{
		//clean up old data
		//get size of old data

	}

	if (dataSize != 0 && dataPtr != NULL)
	{

	}
}


BlockID VoxelChunk::getBlockID(int x, int y, int z)
{
	assert(x >= 0 && x < CHUNK_SIZE);
	assert(y >= 0 && y < CHUNK_SIZE);
	assert(z >= 0 && z < CHUNK_SIZE);

	unsigned int idx = x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z;

	return m_data.m_blockIDs[idx];
}