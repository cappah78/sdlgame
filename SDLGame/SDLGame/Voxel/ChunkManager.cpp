#include "ChunkManager.h"

std::shared_ptr<VoxelChunk> const ChunkManager::getChunk(const glm::ivec3& pos)
{
	if (pos == m_lastReturnedChunkPos)
		return m_lastReturnedChunk;

	auto it = m_loadedChunks.find(pos);
	if (it != m_loadedChunks.end())
	{
		std::shared_ptr<VoxelChunk> chunk = it->second;
		m_lastReturnedChunk = chunk;
		m_lastReturnedChunkPos = pos;
		return chunk;
	}
	else
	{
		std::shared_ptr<VoxelChunk> chunk = loadChunk(pos);
		m_lastReturnedChunk = chunk;
		m_lastReturnedChunkPos = pos;
		return chunk;
	}
}

std::shared_ptr<VoxelChunk> ChunkManager::loadChunk(const glm::ivec3& pos)
{
	//printf("loadchunk: %i, %i, %i \n", pos.x, pos.y, pos.z);
	std::shared_ptr<VoxelChunk> chunk = std::shared_ptr<VoxelChunk>(new VoxelChunk(m_propertyManager, pos));

	m_loadedChunks.insert(std::make_pair(pos, chunk));
	//TODO: chunk generation
	//generateChunk(chunk);

	return chunk;
}

void ChunkManager::unloadChunk(std::shared_ptr<VoxelChunk> chunk)
{
	auto it = m_loadedChunks.find(chunk->m_pos);
	if (it != m_loadedChunks.end())
		m_loadedChunks.erase(it);
}