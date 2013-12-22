#include "LuaChunkGenerator.h"

static const char* LUA_GEN_SCRIPT_PATH = "Assets/Scripts/WorldGen/";

#include <lua.hpp>
#include <LuaBridge.h>

#include <iostream>
#include <stdio.h>

LuaChunkGenerator::LuaChunkGenerator(lua_State* L, int seed)
	: m_L(L)
	, m_seed(seed)
{

}

LuaChunkGenerator::~LuaChunkGenerator()
{

}

void LuaChunkGenerator::generateChunk(VoxelChunk* const chunk)
{
	std::cout << "gananarantatg" << std::endl;
}