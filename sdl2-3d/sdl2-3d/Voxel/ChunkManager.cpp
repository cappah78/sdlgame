#include "ChunkManager.h"

std::unique_ptr<VoxelChunk>& ChunkManager::getChunk(const glm::ivec3& pos)
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

std::unique_ptr<VoxelChunk>& ChunkManager::loadChunk(const glm::ivec3& pos)
{
	m_loadedChunks[pos] = std::unique_ptr<VoxelChunk>(new VoxelChunk(m_propertyManager, pos));

	return m_loadedChunks[pos];
}

void ChunkManager::unloadChunk(const glm::ivec3& chunkPos)
{
	auto it = m_loadedChunks.find(chunkPos);
	if (it != m_loadedChunks.end())
	{
		m_loadedChunks.erase(it);
		setChunkGenerated(chunkPos.x, chunkPos.z, false);
	}
}

bool ChunkManager::isChunkGenerated(int chunkX, int chunkZ) const
{
	auto it = m_generatedChunkSet.find(ChunkPos(chunkX, chunkZ));
	return it != m_generatedChunkSet.end();
}

void ChunkManager::setChunkGenerated(int chunkX, int chunkZ, bool isGenerated)
{
	if (isGenerated)
		m_generatedChunkSet.insert(ChunkPos(chunkX, chunkZ));
	else
	{
		auto it = m_generatedChunkSet.find(ChunkPos(chunkX, chunkZ));
		if (it != m_generatedChunkSet.end())
			m_generatedChunkSet.erase(it);
	}
}