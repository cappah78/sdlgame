#include "VoxelChunk.h"

#include "PropertyManager.h"

static const float RESIZE_MULTIPLIER = 1.5f;

static const Color8888 DEFAULT_LIGHT_LEVEL(0, 0, 0, 255);
static const Color8888 SOLID_BLOCK_COLOR(0, 0, 0, 0);

VoxelChunk::VoxelChunk(PropertyManager& propertyManager, const glm::ivec3& pos)
	: m_pos(pos)
	, m_propertyManager(propertyManager)
	, m_data(0)
	, m_updated(false)
{
	m_blockColors.resize(CHUNK_SIZE_CUBED, DEFAULT_LIGHT_LEVEL);
	m_blockIDs.resize(CHUNK_SIZE_CUBED, 0);
	m_skyVisible.resize(CHUNK_SIZE_CUBED, false);
	m_solid.resize(CHUNK_SIZE_CUBED, false);
}

void VoxelChunk::setBlock(BlockID blockID, const glm::ivec3& blockPos, void* dataPtr, unsigned int dataSize)
{
	unsigned int idx = getBlockIndex(blockPos);

	m_updated = false;

	BlockID& id = m_blockIDs[idx];

	m_propertyManager.getBlockRenderData(blockID);

	//TODO: solid checking
	m_blockColors[idx] = SOLID_BLOCK_COLOR;
	m_solid[idx] = true;

	/*
	//TODO: all the things
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
		//insert new block data
	}
	*/

	id = blockID;
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

//--

VoxelChunk::ChunkDataContainer::ChunkDataContainer(unsigned int initialSize)
	: m_size(initialSize)
	, m_used(0)
{
	m_blockDataPositions.resize(CHUNK_SIZE_CUBED);

	m_dataBegin = malloc(initialSize);
	assert(m_dataBegin && "Failed to allocate");
}

VoxelChunk::ChunkDataContainer::~ChunkDataContainer(){}

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
}

void* VoxelChunk::ChunkDataContainer::get(unsigned int position, unsigned int size)
{
	void* at = (void*) ((char*) m_dataBegin + position);
	return at;
}

/** invalidates all returned positions*/
void VoxelChunk::ChunkDataContainer::remove(unsigned int position, unsigned int size)
{
	void* removeHead = (void*) ((char*) m_dataBegin + position);
	void* removeTail = (void*) ((char*) removeHead + size);

	unsigned int moveSize = m_used - (position + size);
	memmove(removeHead, removeTail, moveSize);
	m_used -= size;

	shiftPositionData(position, size);
}

void VoxelChunk::ChunkDataContainer::shiftPositionData(unsigned int position, unsigned int amount)
{
	// shift all the elements after position by amount to the left.
	for (unsigned int& blockDataPos : m_blockDataPositions)
	{
		if (blockDataPos > position)
			blockDataPos -= amount;
	}
}

void VoxelChunk::ChunkDataContainer::resize(unsigned int newSize)
{
	void* newBegin = malloc(newSize);
	memcpy(newBegin, m_dataBegin, m_used);
	free(m_dataBegin);

	m_dataBegin = newBegin;
	m_size = newSize;
}