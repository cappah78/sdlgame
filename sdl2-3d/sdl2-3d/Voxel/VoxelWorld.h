#pragma once

#include "VoxelChunk.h"

#include "PropertyManager.h"
#include "ChunkManager.h"

#include <vector>
#include <glm\glm.hpp>
#include <noise\noise.h>

#include "../Engine/Utils/noiseutils.h"

#include <memory>

struct lua_State;
class Camera;
class ITextureArray;
class IGraphicsProvider;

static const unsigned int CHUNK_LOAD_RANGE = 16;

class VoxelWorld
{
public:
	VoxelWorld();
	VoxelWorld(const VoxelWorld& copy) = delete;
	~VoxelWorld();

	void update(float deltaSec, const Camera& camera);
	void doBlockUpdate(const glm::ivec3& blockWorldPos);
	void setBlock(BlockID blockID, const glm::ivec3& pos);

	const VoxelBlock& getBlock(const glm::ivec3& pos);
	const BlockProperties& getBlockProperties(BlockID blockID);
	unsigned int VoxelWorld::getSolidSurroundingBlockBits(const glm::ivec3& blockPos);

	const ChunkManager::ChunkMap& getChunks();
	std::unique_ptr<VoxelChunk>& getChunk(const glm::ivec3& chunkPos);

	lua_State* const L() const { return m_L; };

	const std::unique_ptr<ITextureArray>& getTileSet() const { return m_textureArray; };
	const PropertyManager& getPropertyManager() const { return m_propertyManager; };

	inline static glm::ivec3 toChunkPos(const glm::ivec3& blockPos);
	inline static glm::ivec3 toChunkBlockPos(const glm::ivec3& blockPos);

protected:
	static std::map<lua_State* const, VoxelWorld* const> stateWorldMap;

private:

	void doBlockUpdates();
	void initializeLuaWorld();
	void initializeTileSet();
	void generateChunk(const glm::ivec3& chunkPos);

	const BlockID* const getBlockLayer(int height);

	static int L_registerBlockType(lua_State* L);
	static int L_setBlock(BlockID blockID, int x, int y, int z, lua_State* L);
	static int L_setBlockWithData(BlockID blockID, int x, int y, int z, luabridge::LuaRef perBlockData, lua_State* L);
	static int L_getBlock(int x, int y, int z, lua_State* L);

	model::Plane m_worldGenerator;
	float m_timeAccumulator;
	float m_tickDurationSec;

	std::unique_ptr<ITextureArray> m_textureArray;
	ChunkManager m_chunkManager;
	PropertyManager m_propertyManager;
	lua_State* m_L;
	std::vector<glm::ivec3> m_updatedBlockPositions;
};