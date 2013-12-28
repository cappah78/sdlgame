#include "ChunkManager.h"

VoxelChunk* const ChunkManager::getChunk(glm::ivec3& pos)
{
	if (pos == m_lastReturnedChunkPos)
		return m_lastReturnedChunk;

	auto it = m_loadedChunks.find(pos);
	if (it != m_loadedChunks.end())
	{
		VoxelChunk* chunk = it->second;
		m_lastReturnedChunk = chunk;
		m_lastReturnedChunkPos = pos;
		return chunk;
	}
	else
	{
		VoxelChunk* chunk = loadChunk(pos);
		m_lastReturnedChunk = chunk;
		m_lastReturnedChunkPos = pos;
		return chunk;
	}
}

VoxelChunk* ChunkManager::loadChunk(glm::ivec3& pos)
{
	//printf("loadchunk: %i, %i, %i \n", pos.x, pos.y, pos.z);
	VoxelChunk* chunk = new VoxelChunk(m_propertyManager, pos);

	m_loadedChunks.insert(std::make_pair(pos, chunk));
	//TODO:..
	//generateChunk(chunk);

	return chunk;
}

void ChunkManager::unloadChunk(VoxelChunk* chunk)
{
	auto it = m_loadedChunks.find(chunk->m_pos);
	if (it != m_loadedChunks.end())
		m_loadedChunks.erase(it);

	delete chunk;
	chunk = NULL;
}