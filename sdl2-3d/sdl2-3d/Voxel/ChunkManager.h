#ifndef CHUNK_MANAGER_H_
#define CHUNK_MANAGER_H_

#include "VoxelChunk.h"
#include <glm\glm.hpp>
#include <map>

struct IVec3Cmp
{
	bool operator()(const glm::ivec3& vec1, const glm::ivec3& vec2) const
	{
		return (vec1.x + vec1.y + vec1.z) > (vec2.x + vec2.y + vec2.z);
	}
};

template <typename ChunkGenerator>
class ChunkManager
{
public:
	ChunkManager(ChunkGenerator* generator);
	ChunkManager(const ChunkManager& copy) = delete;
	~ChunkManager();

	VoxelChunk* getChunk(int x, int y, int z);
	void unloadChunk(VoxelChunk* chunk);
	VoxelChunk* loadChunk(int x, int y, int z);

	void generateChunk(VoxelChunk* const chunk)
	{
		m_generator->generateChunk(chunk);
	};

private:
	std::map<glm::ivec3, VoxelChunk*, IVec3Cmp> m_loadedChunks;

	ChunkGenerator* m_generator;
};

#endif //CHUNK_MANAGER_H_
