#ifndef VOXEL_WORLD_H_
#define VOXEL_WORLD_H_

#include "VoxelChunk.h"

#include "TextureManager.h"
#include "PropertyManager.h"

#include "LuaChunkGenerator.h"
#include "ChunkManager.h"

#include <vector>
#include <glm\glm.hpp>

struct lua_State;
class TextureArray;

static const unsigned int CHUNK_LOAD_RANGE = 16;

class VoxelWorld
{
public:
	VoxelWorld(TextureManager& textureManager);
	VoxelWorld(const VoxelWorld& copy) = delete;
	~VoxelWorld();

	void setBlock(BlockID blockID, const glm::ivec3& pos);

	BlockID getBlockID(const glm::ivec3& pos);
	BlockIDColor getBlockIDColor(const glm::ivec3& pos);
	BlockIDColorSolid getBlockIDColorSolid(const glm::ivec3& pos);

	std::shared_ptr<VoxelChunk> getChunk(const glm::ivec3& chunkPos);

	const TextureArray* const getTileSet() const { return m_textureArray; };
	const TextureManager& getTextureManager() const { return m_textureManager; };
	const PropertyManager& getPropertyManager() const { return m_propertyManager; };

	const ChunkManager::ChunkMap& getChunks();

	/** Get the lua state */
	lua_State* const L() const { return m_L; };

	inline static glm::ivec3 toChunkPos(const glm::ivec3& blockPos);
	inline static glm::ivec3 toChunkBlockPos(const glm::ivec3& blockPos);

protected:
	static std::map<lua_State* const, VoxelWorld* const> stateWorldMap;

private:
	lua_State* m_L;
	TextureArray* m_textureArray;
	TextureManager& m_textureManager;
	PropertyManager m_propertyManager;

	LuaChunkGenerator m_generator;
	ChunkManager m_chunkManager;

	inline static int fastFloor(float x)
	{
		int i = (int) x;
		return i - (i > x);
	};

	void initializeLuaWorld();
	static int L_registerBlockType(lua_State* L);
	static int L_setBlock(lua_State* L);
};

#endif //VOXEL_WORLD_H_