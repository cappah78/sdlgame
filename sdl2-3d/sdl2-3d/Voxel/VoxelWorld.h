#ifndef VOXEL_WORLD_H_
#define VOXEL_WORLD_H_

#include "VoxelChunk.h"

#include "../Engine/Graphics/TextureManager.h"
#include "PropertyManager.h"

#include "ChunkManager.h"

#include "../Engine/Graphics/GL/GBuffer.h"

#include <vector>
#include <glm\glm.hpp>

#include <noise\noise.h>
#include "../Engine/Utils/noiseutils.h"

struct lua_State;
class Camera;
class TextureArray;

static const unsigned int CHUNK_LOAD_RANGE = 16;

class VoxelWorld
{
public:
	VoxelWorld(TextureManager& textureManager);
	VoxelWorld(const VoxelWorld& copy) = delete;
	~VoxelWorld();

	void setBlock(BlockID blockID, const glm::ivec3& pos);

	const VoxelBlock& getBlock(const glm::ivec3& pos);
	const BlockProperties& getBlockProperties(BlockID blockID);

	std::unique_ptr<VoxelChunk>& getChunk(const glm::ivec3& chunkPos);

	const std::shared_ptr<TextureArray> getTileSet() const { return m_textureArray; };
	const TextureManager& getTextureManager() const { return m_textureManager; };
	const PropertyManager& getPropertyManager() const { return m_propertyManager; };

	void update(float deltaSec, const Camera& camera);
	void doBlockUpdate(const glm::ivec3& blockWorldPos);

	const ChunkManager::ChunkMap& getChunks();

	/** Get the lua state */
	lua_State* const L() const { return m_L; };

	inline static glm::ivec3 toChunkPos(const glm::ivec3& blockPos);
	inline static glm::ivec3 toChunkBlockPos(const glm::ivec3& blockPos);

protected:
	static std::map<lua_State* const, VoxelWorld* const> stateWorldMap;

private:
	model::Plane m_worldGenerator;

	float m_timeAccumulator;
	float m_tickDurationSec;

	lua_State* m_L;
	std::shared_ptr<TextureArray> m_textureArray;
	TextureManager& m_textureManager;
	PropertyManager m_propertyManager;

	ChunkManager m_chunkManager;

	GBuffer m_gbuffer;

	std::vector<glm::ivec3> m_updatedBlockPositions;

	void doBlockUpdates();

	inline static int fastFloor(float x)
	{
		int i = (int) x;
		return i - (i > x);
	};

	BlockID* getBlockLayer(int height);
	void generateChunk(const glm::ivec3& chunkPos);

	void initializeLuaWorld();
	static int L_registerBlockType(lua_State* L);
	static int L_setBlock(BlockID blockID, int x, int y, int z, lua_State* L);
	static int L_setBlockWithData(BlockID blockID, int x, int y, int z, luabridge::LuaRef perBlockData, lua_State* L);
	static int L_getBlock(int x, int y, int z, lua_State* L);
};

#endif //VOXEL_WORLD_H_