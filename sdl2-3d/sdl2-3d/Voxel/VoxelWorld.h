#ifndef VOXEL_WORLD_H_
#define VOXEL_WORLD_H_

#include "VoxelBatch.h"
#include "VoxelChunk.h"

#include "VoxelBlock.h"

#include <vector>
#include <map>
#include <string>
#include <lua.hpp>
#include <LuaBridge.h>

static const unsigned int CHUNK_LOAD_RANGE = 16;


class VoxelWorld
{
public:
	VoxelWorld();
	VoxelWorld(const VoxelWorld& copyMe);
	~VoxelWorld();

	BlockID registerBlockType(const std::string& blockname);
	void setBlock(BlockID blockId, int x, int y, int z);
	lua_State* const L() { return m_L; };
protected:
	static std::map<lua_State* const, VoxelWorld*> stateWorldMap;

private:
	void initializeLuaWorld();
	lua_State* m_L;

	static int L_registerBlockType(lua_State* L);

	//VoxelBatch batch;
	//VoxelChunk chunk;
};

#endif //VOXEL_WORLD_H_