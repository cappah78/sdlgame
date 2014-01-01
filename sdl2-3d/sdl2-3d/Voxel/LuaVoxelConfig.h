#ifndef LUA_VOXEL_CONFIG_H_
#define LUA_VOXEL_CONFIG_H_

//TODO: perhaps move all this to a .lua

static const char* LUA_BLOCK_SCRIPT_DIR_RELATIVE = ".\\Assets\\Scripts\\Blocks\\";
static const char* LUA_BLOCK_SCRIPT_DIR = "Assets\\Scripts\\Blocks\\";

static const char* LUA_WORLD_NAMESPACE = "World";
static const char* LUA_WORLD_REGISTER_FUNCTION = "registerBlockType";
static const char* LUA_WORLD_SETBLOCK_FUNCTION = "setBlock";
static const char* LUA_WORLD_GETBLOCK_FUNCTION = "getBlock";


static const char* LUA_INIT_SCRIPT = "Assets\\Scripts\\Init.lua";
static const char* LUA_INIT_NAMESPACE = "Init";
static const char* LUA_INIT_REGISTER_FUNCTION = "registerBlocks";

static const char* LUA_TICK_DURATION_SEC_NAME = "TickDurationSec";

#endif //LUA_VOXEL_CONFIG_H_