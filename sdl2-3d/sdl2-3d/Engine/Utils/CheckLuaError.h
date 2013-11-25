#ifndef CHECK_LUA_ERROR_H_
#define CHECK_LUA_ERROR_H_

#include <LuaUnityBuild.h>
#include <LuaBridge.h>

#include <stdio.h>
#include <iostream>

#ifdef _DEBUG
#define CHECK_LUA_ERROR(L, status) { checkLuaError(L, status) && (DBG_BREAK(), 1); }
#else // !_DEBUG
#define CHECK_LUA_ERROR(L, status) {  }
#endif // _DEBUG
/**
* Internal function used by macro CHECK_GL_ERROR, use that instead.
*/
bool checkGLError(const char *file, int line);


//custom errors for lua
void checkLuaError(lua_State *L, int status)
{
	if (status != 0) {
		std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1); // remove error message
	}
}


#endif //CHECK_LUA_ERROR_H_
