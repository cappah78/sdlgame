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

static const int SEED = 23;

VoxelWorld::VoxelWorld(TextureManager& textureManager)
	: m_L(luaL_newstate())
	, m_generator(m_L, SEED)
	, m_textureManager(textureManager)
	, m_propertyManager(textureManager)
	, m_chunkManager(m_propertyManager, m_generator)
{
	stateWorldMap.insert(std::make_pair(m_L, this));	// dirty way to retrieve a world object after a lua->c++ call.

	Game::initLua(m_L);
	initializeLuaWorld();
	m_textureArray = m_textureManager.generateTextureArray();

	m_generator.generateMap();
	m_chunkManager.getChunk(glm::ivec3(0, 0, 0));
}

void VoxelWorld::initializeLuaWorld()
{
	luabridge::getGlobalNamespace(m_L)
		.beginNamespace(LUA_WORLD_NAMESPACE)
		.addCFunction(LUA_WORLD_REGISTER_FUNCTION, &L_registerBlockType)
		.addCFunction(LUA_WORLD_SETBLOCK_FUNCTION, &L_setBlock)
		.endNamespace();

	checkLuaError(m_L, luaL_dofile(m_L, LUA_INIT_SCRIPT));

	try 
	{
		luabridge::LuaRef init = luabridge::getGlobal(m_L, LUA_INIT_NAMESPACE);
		init[LUA_INIT_REGISTER_FUNCTION](LUA_BLOCK_SCRIPT_DIR_RELATIVE);	// Run lua Init.registerBlocks()
	}
	catch (luabridge::LuaException const& e) 
	{
		std::cerr << e.what() << std::endl;
	}
}


int VoxelWorld::L_registerBlockType(lua_State* L)
{
	int numArgs = lua_gettop(L);
	assert(numArgs == 1);

	std::string filename = lua_tostring(L, 1);
	std::string blockname(filename, 0, filename.size() - 4);
	std::string scriptDir(LUA_BLOCK_SCRIPT_DIR);
	scriptDir.append(filename);

#ifdef _DEBUG
	std::string luaFileExtention = filename.substr(filename.size() - 4, 4);
	assert(luaFileExtention == ".lua");
#endif //!DEBUG

	checkLuaError(L, luaL_dofile(L, scriptDir.c_str()));
	VoxelWorld::stateWorldMap.at(L)->m_propertyManager.registerBlockType(L, blockname);

	return 0;	// nothing returned
}

int VoxelWorld::L_setBlock(lua_State* L)
{
	int numArgs = lua_gettop(L);
	assert(numArgs == 4);

	int blockID = lua_tointeger(L, 1);

	int x = lua_tointeger(L, -1);
	int y = lua_tointeger(L, -2);
	int z = lua_tointeger(L, -3);

	VoxelWorld* world = VoxelWorld::stateWorldMap.at(L);
	world->setBlock(blockID, glm::ivec3(x, y, z));

	return 0;
}

void VoxelWorld::setBlock(BlockID blockID, const glm::ivec3& pos)
{
	int chunkX = (int) glm::floor((pos.x / (float) CHUNK_SIZE));
	int chunkY = (int) glm::floor((pos.y / (float) CHUNK_SIZE));
	int chunkZ = (int) glm::floor((pos.z / (float) CHUNK_SIZE));

	glm::ivec3 chunkPos(chunkX, chunkY, chunkZ);	
	VoxelChunk* chunk = m_chunkManager.getChunk(chunkPos);

	int blockX = pos.x % (int) CHUNK_SIZE;
	int blockY = pos.y % (int) CHUNK_SIZE;
	int blockZ = pos.z % (int) CHUNK_SIZE;
	if (blockX < 0)
		blockX += (int) CHUNK_SIZE;
	if (blockY < 0)
		blockY += (int) CHUNK_SIZE;
	if (blockZ < 0)
		blockZ += (int) CHUNK_SIZE;

	chunk->setBlock(blockID, blockX, blockY, blockZ);
}

BlockID VoxelWorld::getBlockID(const glm::ivec3& pos) const
{
	int chunkX = (int) glm::floor((pos.x / (float) CHUNK_SIZE));
	int chunkY = (int) glm::floor((pos.y / (float) CHUNK_SIZE));
	int chunkZ = (int) glm::floor((pos.z / (float) CHUNK_SIZE));

	glm::ivec3 chunkPos(chunkX, chunkY, chunkZ);
	VoxelChunk* chunk = m_chunkManager.getChunk(chunkPos);

	int blockX = pos.x % (int) CHUNK_SIZE;
	int blockY = pos.y % (int) CHUNK_SIZE;
	int blockZ = pos.z % (int) CHUNK_SIZE;

	if (blockX < 0)
		blockX += (int) CHUNK_SIZE;
	if (blockY < 0)
		blockY += (int) CHUNK_SIZE;
	if (blockZ < 0)
		blockZ += (int) CHUNK_SIZE;


	return chunk->getBlockID(blockX, blockY, blockZ);
}

const ChunkManager::ChunkMap& VoxelWorld::getChunks() const
{
	return m_chunkManager.getLoadedChunkMap();
}

VoxelWorld::~VoxelWorld()
{

}


