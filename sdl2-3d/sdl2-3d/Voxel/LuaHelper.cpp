#include "LuaHelper.h"

#include "../Game.h"
#include "../Engine/Utils/CheckLuaError.h"

#include <iostream>
#include <stdio.h>
#include <vector>
#include <lua.hpp>
#include <LuaBridge.h>

static const std::string SCRIPT_PATH("Assets/Scripts/");
static const std::string SCRIPT_FILE_EXT(".lua");
static const std::string BLOCK_NAMESPACE("Blocks.");
static const std::string LUA_INIT(" = {}");

void LuaHelper::createBlockType(const char* implementationName)
{
	std::string implementationStr(implementationName);
	std::size_t found = implementationStr.find("class ");
	if (found == std::string::npos) //null implementation
	{

	}
	else
	{
		std::string name = implementationStr.substr(6); //substr to remove 'class '

		std::string scriptPath(SCRIPT_PATH);
		scriptPath.append(name);
		scriptPath.append(SCRIPT_FILE_EXT);

		std::string initLuaBlock(BLOCK_NAMESPACE);
		initLuaBlock.append(name);
		initLuaBlock.append(LUA_INIT);
		luaL_dostring(Game::L, initLuaBlock.c_str());

		checkLuaError(Game::L, luaL_dofile(Game::L, scriptPath.c_str()));

		luabridge::LuaRef ref = luabridge::getGlobal(Game::L, "Blocks");
		if (ref.isNil())
			return;
		luabridge::LuaRef ref2 = ref[name];
		if (ref2.isNil())
			return;
		luabridge::LuaRef ref3 = ref2["test"];
		if (ref3.isNil())
			return;

		try {
			ref3();
		}
		catch (luabridge::LuaException const& e) {
			std::cout << e.what() << std::endl;
		}
	}
}