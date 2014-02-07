#pragma once

#include <lua.hpp>
#include <LuaBridge.h>

#include <stdio.h>
#include <iostream>

//custom errors for lua
inline void checkLuaError(lua_State *L, int status)
{
	if (status != 0) {
		std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1); // remove error message
	}
}