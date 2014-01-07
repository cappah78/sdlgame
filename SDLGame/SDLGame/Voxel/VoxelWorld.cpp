#include "VoxelWorld.h"

#include "LuaVoxelConfig.h"

#include "../Game.h"
#include "../Engine/Utils/CheckLuaError.h"
#include "../Engine/Graphics/Color8888.h"
#include "../Engine/Graphics/GL/TextureArray.h"

#include <lua.hpp>
#include <LuaBridge.h>

std::map<lua_State* const, VoxelWorld* const> VoxelWorld::stateWorldMap;

//TODO: remove constant
static const unsigned int BLOCK_TEX_RES = 16;
static const float BLOCK_TO_CHUNK = 1 / (float) CHUNK_SIZE;

static const int SEED = 23;

VoxelWorld::VoxelWorld(TextureManager& textureManager)
	: m_L(luaL_newstate())
	, m_generator(m_L, SEED)
	, m_textureManager(textureManager)
	, m_propertyManager(textureManager)
	, m_chunkManager(m_propertyManager, m_generator)
	, m_timeAccumulator(0)
	, m_tickDurationSec(0)
{
	stateWorldMap.insert(std::make_pair(m_L, this));	// dirty way to retrieve a world object after a lua->c++ call.

	Game::initLua(m_L);
	initializeLuaWorld();
	m_textureArray = m_textureManager.generateTextureArray();

	m_generator.generateMap();
}

void VoxelWorld::initializeLuaWorld()
{
	luabridge::getGlobalNamespace(m_L)
		.beginNamespace(LUA_WORLD_NAMESPACE)
		.addCFunction(LUA_WORLD_REGISTER_FUNCTION, &L_registerBlockType)//Add World.registerBlockType
		//.addCFunction(LUA_WORLD_SETBLOCK_FUNCTION, &L_setBlock)				
		.addFunction(LUA_WORLD_SETBLOCK_FUNCTION, &L_setBlock)//add World.setBlock
		.addFunction(LUA_WORLD_SETBLOCK_WITH_DATA_FUNCTION, &L_setBlockWithData)//add World.setBlock
		.addFunction(LUA_WORLD_GETBLOCK_FUNCTION, &L_getBlock)
		.endNamespace();

	checkLuaError(m_L, luaL_dofile(m_L, LUA_INIT_SCRIPT));

	luabridge::LuaRef init = luabridge::getGlobal(m_L, LUA_INIT_NAMESPACE);
	m_tickDurationSec = luabridge::getGlobal(m_L, LUA_TICK_DURATION_SEC_NAME);
}

int VoxelWorld::L_registerBlockType(lua_State* L)
{
	int numArgs = lua_gettop(L);
	assert(numArgs == 1);

	std::string filename = lua_tostring(L, 1);
	std::string blockname(filename, 0, filename.size() - 4);	//remove .lua extention

	VoxelWorld::stateWorldMap.at(L)->m_propertyManager.registerBlockType(L, blockname);

	return 0;	// nothing returned
}

int VoxelWorld::L_setBlock(BlockID blockID, int x, int y, int z, lua_State* L)
{
	assert(lua_gettop(L) == 4 && "World.setBlock requires 4 arguments (BlockID, x, y, z)");	//check if numargs == 4

	VoxelWorld* world = VoxelWorld::stateWorldMap.at(L);
	world->setBlock(blockID, glm::ivec3(x, y, z));

	return 0;
}

int VoxelWorld::L_setBlockWithData(BlockID blockID, int x, int y, int z, luabridge::LuaRef perBlockData, lua_State* L)
{
	printf("lolol");
	VoxelWorld* world = VoxelWorld::stateWorldMap.at(L);
	
	world->setBlock(blockID, glm::ivec3(x, y, z));

	return 0;
}


int VoxelWorld::L_getBlock(int x, int y, int z, lua_State* L)
{
	VoxelWorld* world = VoxelWorld::stateWorldMap.at(L);
	const VoxelBlock& block = world->getBlock(glm::ivec3(x, y, z));

	return block.id;
}

void VoxelWorld::update(float deltaSec)
{
	m_timeAccumulator += deltaSec;
	while (m_timeAccumulator >= m_tickDurationSec)
	{
		m_timeAccumulator -= m_tickDurationSec;

		for (auto it : m_chunkManager.getLoadedChunkMap())
		{
			it.second->doBlockUpdate();
		}
	}
}

void VoxelWorld::setBlock(BlockID blockID, const glm::ivec3& pos)
{
	const glm::ivec3& chunkPos = toChunkPos(pos);
	const std::shared_ptr<VoxelChunk>& chunk = m_chunkManager.getChunk(chunkPos);
	const glm::ivec3& blockPos = toChunkBlockPos(pos);

	chunk->setBlock(blockID, blockPos);
}

const VoxelBlock& VoxelWorld::getBlock(const glm::ivec3& pos)
{
	const glm::ivec3& chunkPos = toChunkPos(pos);
	const std::shared_ptr<VoxelChunk> chunk = m_chunkManager.getChunk(chunkPos);
	const glm::ivec3& blockPos = toChunkBlockPos(pos);

	return chunk->getBlock(blockPos);
}

const BlockProperties& VoxelWorld::getBlockProperties(BlockID blockID)
{
	return m_propertyManager.getBlockProperties(blockID);
}

inline glm::ivec3 VoxelWorld::toChunkPos(const glm::ivec3& blockPos)
{
	int chunkX = fastFloor(blockPos.x * BLOCK_TO_CHUNK);
	int chunkY = fastFloor(blockPos.y * BLOCK_TO_CHUNK);
	int chunkZ = fastFloor(blockPos.z * BLOCK_TO_CHUNK);
	return glm::ivec3(chunkX, chunkY, chunkZ);
}

inline glm::ivec3 VoxelWorld::toChunkBlockPos(const glm::ivec3& blockPos)
{
	int blockX = blockPos.x % (int) CHUNK_SIZE;
	int blockY = blockPos.y % (int) CHUNK_SIZE;
	int blockZ = blockPos.z % (int) CHUNK_SIZE;
	if (blockX < 0)
		blockX += (int) CHUNK_SIZE;
	if (blockY < 0)
		blockY += (int) CHUNK_SIZE;
	if (blockZ < 0)
		blockZ += (int) CHUNK_SIZE;
	return glm::ivec3(blockX, blockY, blockZ);
}

const ChunkManager::ChunkMap& VoxelWorld::getChunks()
{
	return m_chunkManager.getLoadedChunkMap();
}

std::shared_ptr<VoxelChunk> VoxelWorld::getChunk(const glm::ivec3& chunkPos)
{
	return m_chunkManager.getChunk(chunkPos);
}

VoxelWorld::~VoxelWorld()
{

}


