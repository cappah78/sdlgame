#pragma once

#include <lua.hpp>
#include <LuaBridge.h>

//custom errors for lua
inline void checkLuaError(lua_State *L, int status)
{
	if (status != 0) {
		printf("-- %s\n", lua_tostring(L, -1));
		lua_pop(L, 1); // remove error message
	}
}