#pragma once

//TODO: perhaps move all this to a .lua

static const char* const LUA_BLOCK_SCRIPT_DIR_RELATIVE = ".\\Assets\\Scripts\\Blocks\\";
static const char* const LUA_BLOCK_SCRIPT_DIR = "Assets\\Scripts\\Blocks\\";

static const char* const LUA_WORLD_NAMESPACE = "CPP_World";
static const char* const LUA_WORLD_REGISTER_FUNCTION = "registerBlockType";
static const char* const LUA_WORLD_SETBLOCK_FUNCTION = "setBlock";
static const char* const LUA_WORLD_SETBLOCK_WITH_DATA_FUNCTION = "setBlockD";
static const char* const LUA_WORLD_GETBLOCK_FUNCTION = "getBlock";

static const char* const LUA_INIT_SCRIPT = "Assets\\Scripts\\Init.lua";
static const char* const LUA_INIT_NAMESPACE = "Init";
static const char* const LUA_TICK_DURATION_SEC_NAME = "TickDurationSec";

static const char* const LUA_BLOCKS_NAMESPACE = "Blocks";
static const char* const LUA_INT_NAME = "int";
static const char* const LUA_BOOL_NAME = "bool";
static const char* const LUA_FLOAT_NAME = "float";
static const char* const LUA_TICK_COUNT_NAME = "tickCount";
static const char* const LUA_BLOCK_X_NAME = "x";
static const char* const LUA_BLOCK_Y_NAME = "y";
static const char* const LUA_BLOCK_Z_NAME = "z";