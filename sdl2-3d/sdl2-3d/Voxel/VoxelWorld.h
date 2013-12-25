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

	void setBlock(BlockID blockID, glm::ivec3& pos);

	const ChunkManager::ChunkMap& getChunks() const;
	TextureManager& getTextureManager();
	const PropertyManager& getPropertyManager() const { return m_propertyManager; };
	lua_State* const L() const { return m_L; };

	const TextureArray* const getTileSet() const { return m_textureArray; };

protected:
	static std::map<lua_State* const, VoxelWorld* const> stateWorldMap;

private:
	lua_State* m_L;
	TextureArray* m_textureArray;
	TextureManager& m_textureManager;
	PropertyManager m_propertyManager;

	LuaChunkGenerator m_generator;
	ChunkManager m_chunkManager;

	void initializeLuaWorld();
	static int L_registerBlockType(lua_State* L);
	static int L_setBlock(lua_State* L);
};

#endif //VOXEL_WORLD_H_