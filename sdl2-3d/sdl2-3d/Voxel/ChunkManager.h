#ifndef CHUNK_MANAGER_H_
#define CHUNK_MANAGER_H_

#include "VoxelChunk.h"
#include "LuaChunkGenerator.h"

#include <glm\glm.hpp>
#include <unordered_map>
#include <assert.h>

#include "../Engine/Utils/Comparables.h"

#include "PropertyManager.h"

class ChunkManager
{
public:
	typedef std::unordered_map<const glm::ivec3, VoxelChunk* const, IVec3Hash> ChunkMap;

	ChunkManager(PropertyManager& propertyManager, LuaChunkGenerator& generator) 
		: m_propertyManager(propertyManager)
		, m_generator(generator)
		, m_lastReturnedChunkPos(-99999999999, -99999999999, -99999999999)	//just some bogus initial value for performance sake
		, m_lastReturnedChunk(NULL)
	{};
	ChunkManager(const ChunkManager& copy) = delete;
	~ChunkManager() {};

	VoxelChunk* const getChunk(glm::ivec3& pos);
	void unloadChunk(VoxelChunk* chunk);
	VoxelChunk* loadChunk(glm::ivec3& pos);
	const ChunkMap& getLoadedChunkMap() const { return m_loadedChunks; };

private:

	void generateChunk(VoxelChunk* const chunk)
	{
		m_generator.generateChunk(chunk);
	};

	/** Keeping track of the last returned chunk because its common to retrieve the same chunk multiple times */
	glm::ivec3 m_lastReturnedChunkPos;
	VoxelChunk* m_lastReturnedChunk;

	ChunkMap m_loadedChunks;
	LuaChunkGenerator& m_generator;
	PropertyManager& m_propertyManager;
};

#endif //CHUNK_MANAGER_H_
