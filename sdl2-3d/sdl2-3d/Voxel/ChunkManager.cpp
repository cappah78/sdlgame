#include "ChunkManager.h"

template <typename ChunkGenerator>
ChunkManager<ChunkGenerator>::ChunkManager(ChunkGenerator* generator)
	: m_generator(generator)
	, m_loadedChunks(IVec3Cmp())
{

}

template <typename ChunkGenerator>
VoxelChunk* ChunkManager<ChunkGenerator>::getChunk(int x, int y, int z)
{
	auto it = m_loadedChunks.find(glm::ivec3(x, y, z));
	if (it != m_loadedChunks.end())
	{
		return it->second;
	}
	else
	{
		return loadChunk(x, y, z);
	}
}

template <typename ChunkGenerator>
VoxelChunk* ChunkManager<ChunkGenerator>::loadChunk(int x, int y, int z)
{
	VoxelChunk* chunk = new VoxelChunk(x, y, z);
	generateChunk(chunk);

	m_loadedChunks.insert(std::make_pair(glm::ivec3(x, y, z), chunk));

	return chunk;
}

template <typename ChunkGenerator>
void ChunkManager<ChunkGenerator>::unloadChunk(VoxelChunk* chunk)
{
	auto it = m_loadedChunks.find(glm::ivec3(chunk->m_chunkX, chunk->m_chunkY, chunk->m_chunkZ));
	if (it != m_loadedChunks.end())
		m_loadedChunks.erase(it);

	delete chunk;
	chunk = NULL;
}