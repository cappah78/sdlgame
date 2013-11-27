#ifndef LUA_HELPER_H_
#define LUA_HELPER_H_

#include <string>

class LuaHelper
{
public:
	static void createBlockType(const char* implmentationName);
private:
	LuaHelper() {};
	~LuaHelper() {};
	LuaHelper(const LuaHelper& copy) {};
};


#endif //LUA_HELPER_H_
