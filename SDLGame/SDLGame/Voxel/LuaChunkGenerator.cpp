#include "LuaChunkGenerator.h"

#include "../Engine/Utils/CheckLuaError.h"

static const char* LUA_GEN_SCRIPT_PATH = "Assets/Scripts/WorldGen/GenTest.lua";

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
	//TODO: per chunk map generation
	//luaL_dofile(m_L, LUA_GEN_SCRIPT_PATH);
}

void LuaChunkGenerator::generateMap()
{
	//TODO: temp world gen..
	checkLuaError(m_L, luaL_dofile(m_L, LUA_GEN_SCRIPT_PATH));
}