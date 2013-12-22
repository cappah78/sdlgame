#include "ChunkManager.h"

VoxelChunk* ChunkManager::getChunk(glm::ivec3& pos)
{
	auto it = m_loadedChunks.find(pos);
	if (it != m_loadedChunks.end())
	{
		return it->second;
	}
	else
	{
		return loadChunk(pos);
	}
}

VoxelChunk* ChunkManager::loadChunk(glm::ivec3& pos)
{
	if (pos == m_lastLoadedChunkPos)
	{
		return m_lastLoadedChunk;
	}
	else
	{
		m_lastLoadedChunkPos = pos;
		VoxelChunk* chunk = new VoxelChunk(m_propertyManager, pos);
		generateChunk(chunk);

		m_loadedChunks.insert(std::make_pair(pos, chunk));
		return chunk;
	}
}

void ChunkManager::unloadChunk(VoxelChunk* chunk)
{
	auto it = m_loadedChunks.find(chunk->m_pos);
	if (it != m_loadedChunks.end())
		m_loadedChunks.erase(it);

	delete chunk;
	chunk = NULL;
}