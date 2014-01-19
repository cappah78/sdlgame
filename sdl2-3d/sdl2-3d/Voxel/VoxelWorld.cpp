#include "VoxelWorld.h"

#include "LuaVoxelConfig.h"

#include "../Game.h"
#include "../Engine/Utils/CheckLuaError.h"
#include "../Engine/Graphics/Color8888.h"
#include "../Engine/Graphics/GL/TextureArray.h"
#include "../Engine/Graphics/Camera.h"

#include <lua.hpp>
#include <LuaBridge.h>


std::map<lua_State* const, VoxelWorld* const> VoxelWorld::stateWorldMap;

//TODO: remove constant
static const unsigned int BLOCK_TEX_RES = 16;
static const float BLOCK_TO_CHUNK = 1 / (float) CHUNK_SIZE;

const double blocksPerUnit = 492.0;


VoxelWorld::VoxelWorld(TextureManager& textureManager)
	: m_L(luaL_newstate())
	, m_textureManager(textureManager)
	, m_propertyManager(textureManager)
	, m_chunkManager(m_propertyManager)
	, m_timeAccumulator(0)
	, m_tickDurationSec(1/20.0f)
	, m_gbuffer(0)
{
	stateWorldMap.insert(std::make_pair(m_L, this));	// dirty way to retrieve a world object after a lua->c++ call.

	Game::initLua(m_L);
	initializeLuaWorld();
	m_textureArray = m_textureManager.generateTextureArray();


	////////////////Map generation//////////////	//WIP!
	// TODO: remove statics //
	////////////////////////////////////////////
	static module::Perlin perlin;
	perlin.SetOctaveCount(4);
	perlin.SetFrequency(2.5);

	static module::Turbulence turbulence;
	turbulence.SetFrequency(4.0);
	turbulence.SetPower(0.2);
	turbulence.SetSourceModule(0, perlin);

	static module::ScaleBias scale;
	scale.SetSourceModule(0, turbulence);
	scale.SetScale(40.0);

	m_worldGenerator.SetModule(scale);
	//////////////////////////////////////////////
}

static const unsigned int MAX_GENERATE_TIME_MS = 2;

void VoxelWorld::update(float deltaSec, const Camera& camera)
{
	unsigned int start = Game::getSDLTicks();

	float loadDistance = (camera.m_far / (float) CHUNK_SIZE) - 1.0f;
	float unloadDistance = glm::sqrt(loadDistance * loadDistance + loadDistance * loadDistance) + 1.0f;
	float unloadDistanceSqr = unloadDistance * unloadDistance;
	glm::ivec3 cameraChunkPos = toChunkPos(glm::ivec3(camera.m_position));

	const ChunkManager::ChunkMap& chunkMap = m_chunkManager.getLoadedChunkMap();

	std::vector<glm::ivec3> toUnload;
	for (const std::pair<const glm::ivec3, std::unique_ptr<VoxelChunk>>& posChunk : chunkMap)
	{
		glm::vec3 dst = glm::vec3(cameraChunkPos - posChunk.first);
		float sqrDst = glm::dot(dst, dst);
		if (sqrDst > unloadDistanceSqr)
		{
			toUnload.push_back(posChunk.first);
		}
		if (Game::getSDLTicks() - start > MAX_GENERATE_TIME_MS)
			break;
	}

	for (const glm::ivec3& vec : toUnload)
	{
		m_chunkManager.unloadChunk(vec);

		if (Game::getSDLTicks() - start > MAX_GENERATE_TIME_MS)
			return;
	}

	glm::ivec3 minRange = cameraChunkPos - (int) loadDistance;
	glm::ivec3 maxRange = cameraChunkPos + (int) loadDistance;
	for (int x = minRange.x; x < maxRange.x; ++x)
	{
		for (int z = minRange.z; z < maxRange.z; ++z)
		{
			if (!m_chunkManager.isChunkGenerated(x, z))	//if not, no chunk has been generated yet.
			{
				m_chunkManager.setChunkGenerated(x, z);
				generateChunk(glm::ivec3(x, 0, z));
			}

			if (Game::getSDLTicks() - start > MAX_GENERATE_TIME_MS)
				return;
		}
	}
}

static const int SURFACE_DEPTH = 5;

BlockID* VoxelWorld::getBlockLayer(int height)
{
	BlockID* ids = new BlockID[SURFACE_DEPTH];

	if (height >= 30)
	{
		ids[0] = m_propertyManager.getBlockID("SnowBlock");
		ids[1] = m_propertyManager.getBlockID("GrassSnowBlock");
		ids[2] = m_propertyManager.getBlockID("DirtBlock");
		ids[3] = m_propertyManager.getBlockID("StoneBlock");
		ids[4] = m_propertyManager.getBlockID("StoneBlock");
	}
	else if (height >= 20 && height < 30)
	{
		ids[0] = m_propertyManager.getBlockID("GrassSnowBlock");
		ids[1] = m_propertyManager.getBlockID("DirtBlock");
		ids[2] = m_propertyManager.getBlockID("StoneBlock");
		ids[3] = m_propertyManager.getBlockID("StoneBlock");
		ids[4] = m_propertyManager.getBlockID("StoneBlock");
	}
	else if(height >= -12 && height < 20)
	{
		ids[0] = m_propertyManager.getBlockID("GrassBlock");
		ids[1] = m_propertyManager.getBlockID("DirtBlock");
		ids[2] = m_propertyManager.getBlockID("StoneBlock");
		ids[3] = m_propertyManager.getBlockID("StoneBlock");
		ids[4] = m_propertyManager.getBlockID("StoneBlock");
	}
	else if(height <= -12 && height > -18)
	{
		ids[0] = m_propertyManager.getBlockID("SandBlock");
		ids[1] = m_propertyManager.getBlockID("SandBlock");
		ids[2] = m_propertyManager.getBlockID("DirtBlock");
		ids[3] = m_propertyManager.getBlockID("StoneBlock");
		ids[4] = m_propertyManager.getBlockID("StoneBlock");
	}
	else if(height <= -18)
	{
		ids[0] = m_propertyManager.getBlockID("WaterBlock");
		ids[1] = m_propertyManager.getBlockID("WaterBlock");
		ids[2] = m_propertyManager.getBlockID("WaterBlock");
		ids[3] = m_propertyManager.getBlockID("StoneBlock");
		ids[4] = m_propertyManager.getBlockID("StoneBlock");
	}

	return ids;
}

void VoxelWorld::generateChunk(const glm::ivec3& chunkPos)
{
	const int numBlockIDS = m_propertyManager.getNumRegisteredBlocks();

	glm::ivec3 from = chunkPos * (int) CHUNK_SIZE;
	glm::ivec3 to = from + (int) CHUNK_SIZE;
	for (int x = from.x; x < to.x; ++x)
	{
		for (int z = from.z; z < to.z; ++z)
		{
			int height = (int) m_worldGenerator.GetValue(x / blocksPerUnit, z / blocksPerUnit);

			int blockSampleHeight;
			if (height < -15)
				blockSampleHeight = height;
			else
				blockSampleHeight = height + (int) (glm::sin(x / 0.0123) * 2.0f) + (int) (glm::cos(z / 0.0123) * 2.0f);

			BlockID* ids = getBlockLayer(blockSampleHeight);

			int idx = SURFACE_DEPTH;
			for (int y = height - SURFACE_DEPTH; y < height; ++y)
			{
				//TODO: optimize
				setBlock(ids[--idx], glm::ivec3(x, y, z));
			}
			delete[] ids;
		}
	}
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

//TODO:..
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

void VoxelWorld::setBlock(BlockID blockID, const glm::ivec3& pos)
{
	const glm::ivec3& chunkPos = toChunkPos(pos);
	const std::unique_ptr<VoxelChunk>& chunk = m_chunkManager.getChunk(chunkPos);
	const glm::ivec3& blockPos = toChunkBlockPos(pos);

	//if block is on edge, update touching other chunks
	if (blockPos.x == 0)
		m_chunkManager.getChunk(chunkPos + glm::ivec3(-1, 0, 0))->m_updated = false;
	if (blockPos.x == CHUNK_SIZE - 1)
		m_chunkManager.getChunk(chunkPos + glm::ivec3(1, 0, 0))->m_updated = false;
	if (blockPos.y == 0)
		m_chunkManager.getChunk(chunkPos + glm::ivec3(0, -1, 0))->m_updated = false;
	if (blockPos.y == CHUNK_SIZE - 1)
		m_chunkManager.getChunk(chunkPos + glm::ivec3(0, 1, 0))->m_updated = false;
	if (blockPos.z == 0)
		m_chunkManager.getChunk(chunkPos + glm::ivec3(0, 0, -1))->m_updated = false;
	if (blockPos.z == CHUNK_SIZE - 1)
		m_chunkManager.getChunk(chunkPos + glm::ivec3(0, 0, 1))->m_updated = false;

	chunk->setBlock(blockID, blockPos);
}

const VoxelBlock& VoxelWorld::getBlock(const glm::ivec3& pos)
{
	const glm::ivec3& chunkPos = toChunkPos(pos);
	const std::unique_ptr<VoxelChunk>& chunk = m_chunkManager.getChunk(chunkPos);
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

std::unique_ptr<VoxelChunk>& VoxelWorld::getChunk(const glm::ivec3& chunkPos)
{
	return m_chunkManager.getChunk(chunkPos);
}

VoxelWorld::~VoxelWorld()
{
	//TODO: fix breakpoint trigger O.o? somethin not getting deleted properly
}


