#ifndef VOXEL_WORLD_H_
#define VOXEL_WORLD_H_

#include "VoxelChunk.h"
#include "TextureManager.h"
#include "PropertyManager.h"

#include <vector>
#include <map>
#include <string>
#include <lua.hpp>
#include <LuaBridge.h>

static const unsigned int CHUNK_LOAD_RANGE = 16;

class VoxelWorld
{
public:
	VoxelWorld(TextureManager& textureManager);
	VoxelWorld(const VoxelWorld& copy) = delete;
	~VoxelWorld();

	const std::vector<const VoxelChunk>& getChunks() const;
	TextureManager& getTextureManager();
	const PropertyManager& getPropertyManager() const;

	lua_State* const L() const { return m_L; };

protected:
	static std::map<lua_State* const, VoxelWorld*> stateWorldMap;

private:
	std::vector<VoxelChunk> m_chunks;
	lua_State* m_L;
	TextureManager& m_textureManager;
	PropertyManager m_propertyManager;

	void initializeLuaWorld();
	static int L_registerBlockType(lua_State* L);
};

#endif //VOXEL_WORLD_H_