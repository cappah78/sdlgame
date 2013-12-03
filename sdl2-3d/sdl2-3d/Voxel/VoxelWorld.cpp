#include "VoxelWorld.h"

#include "../Game.h"
#include "../Engine/Utils/CheckLuaError.h"

static const char* LUA_BLOCK_SCRIPT_DIR_RELATIVE = ".\\Assets\\Scripts\\Blocks\\";
static const char* LUA_BLOCK_SCRIPT_DIR = "Assets\\Scripts\\Blocks\\";

static const char* LUA_WORLD_NAMESPACE = "World";
static const char* LUA_WORLD_REGISTER_FUNCTION = "registerBlockType";

static const char* LUA_INIT_SCRIPT = "Assets\\Scripts\\Init.lua";
static const char* LUA_INIT_NAMESPACE = "Init";
static const char* LUA_INIT_REGISTER_FUNCTION = "registerBlocks";

std::map<lua_State* const, VoxelWorld*> VoxelWorld::stateWorldMap;

VoxelWorld::VoxelWorld()
	: m_L(luaL_newstate())
{
	stateWorldMap.insert(std::make_pair(m_L, this));

	Game::initLua(m_L);
	initializeLuaWorld();
}

int VoxelWorld::L_registerBlockType(lua_State* L)
{
	int numArgs = lua_gettop(L);
	assert(numArgs == 1);

	std::string filename = lua_tostring(L, 1);

#ifdef _DEBUG
	std::string luaFileExtention = filename.substr(filename.size() - 4, 4);
	assert(luaFileExtention == ".lua");
#endif //!DEBUG

	std::string scriptDir(LUA_BLOCK_SCRIPT_DIR);
	scriptDir.append(filename);
	checkLuaError(L, luaL_dofile(L, scriptDir.c_str()));

	std::string blockName(filename, 0, filename.size() - 4);

	std::cout << "da: " << blockName << std::endl;
	BlockID blockId = VoxelWorld::stateWorldMap.at(L)->registerBlockType(blockName);
	lua_pushinteger(L, blockId);

	return 1;
}



BlockID VoxelWorld::registerBlockType(const std::string& blockname)
{
	return 2;
}

void VoxelWorld::initializeLuaWorld()
{
	luabridge::getGlobalNamespace(m_L)
		.beginNamespace(LUA_WORLD_NAMESPACE)
		.addCFunction(LUA_WORLD_REGISTER_FUNCTION, &L_registerBlockType)
		.endNamespace();

	checkLuaError(m_L, luaL_dofile(m_L, LUA_INIT_SCRIPT));

	luabridge::LuaRef init = luabridge::getGlobal(m_L, LUA_INIT_NAMESPACE);

	try {
		init[LUA_INIT_REGISTER_FUNCTION](LUA_BLOCK_SCRIPT_DIR_RELATIVE);	// Run lua Init.registerBlocks()
	}
	catch (luabridge::LuaException const& e) {
		std::cout && e.what();
	}
}

VoxelWorld::VoxelWorld(const VoxelWorld& copyMe)
: m_L(luaL_newstate())
{
	assert(false);
}

VoxelWorld::~VoxelWorld()
{

}


