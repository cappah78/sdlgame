#ifndef CHUNK_MANAGER_H_
#define CHUNK_MANAGER_H_

#include "VoxelChunk.h"

#include <glm\glm.hpp>
#include <unordered_map>
#include <assert.h>
#include <memory>

#include "../Engine/Utils/Comparables.h"

#include "PropertyManager.h"

class ChunkManager
{
public:
	typedef std::unordered_map<const glm::ivec3, std::unique_ptr<VoxelChunk>, IVec3Hash> ChunkMap;

	ChunkManager(PropertyManager& propertyManager) 
		: m_propertyManager(propertyManager)
	{};
	ChunkManager(const ChunkManager& copy) = delete;
	~ChunkManager() {};

	std::unique_ptr<VoxelChunk>& getChunk(const glm::ivec3& pos);
	void unloadChunk(const glm::ivec3& chunkPos);
	const ChunkMap& getLoadedChunkMap() const { return m_loadedChunks; };

private:
	std::unique_ptr<VoxelChunk>& loadChunk(const glm::ivec3& pos);

	ChunkMap m_loadedChunks;
	PropertyManager& m_propertyManager;
};

#endif //CHUNK_MANAGER_H_
