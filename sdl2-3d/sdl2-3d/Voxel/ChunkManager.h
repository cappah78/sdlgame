#ifndef CHUNK_MANAGER_H_
#define CHUNK_MANAGER_H_

#include "VoxelChunk.h"
#include "LuaChunkGenerator.h"

#include <glm\glm.hpp>
#include <map>
#include <assert.h>

#include "PropertyManager.h"

/** Comparable so ivec3 can be used in maps*/
struct IVec3Cmp
{
	bool operator()(const glm::ivec3& vec1, const glm::ivec3& vec2) const
	{
		return (vec1.x + vec1.y + vec1.z) > (vec2.x + vec2.y + vec2.z);
	}
};

class ChunkManager
{
public:
	ChunkManager(PropertyManager& propertyManager, LuaChunkGenerator& generator) 
		: m_propertyManager(propertyManager)
		, m_generator(generator)
		, m_loadedChunks(IVec3Cmp()) 
	{};
	ChunkManager(const ChunkManager& copy) = delete;
	~ChunkManager() {};

	VoxelChunk* getChunk(glm::ivec3& pos);
	void unloadChunk(VoxelChunk* chunk);
	VoxelChunk* loadChunk(glm::ivec3& pos);

private:

	void generateChunk(VoxelChunk* const chunk)
	{
		m_generator.generateChunk(chunk);
	};

	/** Keeping track of the last returned chunk because its common to retrieve the same chunk multiple times */
	glm::ivec3 m_lastLoadedChunkPos;
	VoxelChunk* m_lastLoadedChunk;

	std::map<glm::ivec3, VoxelChunk*, IVec3Cmp> m_loadedChunks;
	LuaChunkGenerator& m_generator;
	PropertyManager& m_propertyManager;
};

#endif //CHUNK_MANAGER_H_
