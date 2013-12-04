#include "VoxelWorld.h"

#include "LuaVoxelConfig.h"

#include "../Game.h"
#include "../Engine/Utils/CheckLuaError.h"

std::map<lua_State* const, VoxelWorld*> VoxelWorld::stateWorldMap;

//TODO: remove constant
static const unsigned int BLOCK_TEX_RES = 16;

VoxelWorld::VoxelWorld(TextureManager& textureManager)
	: m_L(luaL_newstate())
	, m_textureManager(textureManager)
	, m_propertyManager(textureManager)
{
	stateWorldMap.insert(std::make_pair(m_L, this));

	Game::initLua(m_L);
	initializeLuaWorld();
	BlockID id = m_propertyManager.getBlockID("StoneBlock");
	std::cout << id << std::endl;
}

void VoxelWorld::initializeLuaWorld()
{
	luabridge::getGlobalNamespace(m_L)
		.beginNamespace(LUA_WORLD_NAMESPACE)
		.addCFunction(LUA_WORLD_REGISTER_FUNCTION, &L_registerBlockType)
		.endNamespace();

	checkLuaError(m_L, luaL_dofile(m_L, LUA_INIT_SCRIPT));

	try 
	{
		luabridge::LuaRef init = luabridge::getGlobal(m_L, LUA_INIT_NAMESPACE);
		init[LUA_INIT_REGISTER_FUNCTION](LUA_BLOCK_SCRIPT_DIR_RELATIVE);	// Run lua Init.registerBlocks()
	}
	catch (luabridge::LuaException const& e) 
	{
		std::cout && e.what();
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

	return 0;
}

VoxelWorld::~VoxelWorld()
{

}


