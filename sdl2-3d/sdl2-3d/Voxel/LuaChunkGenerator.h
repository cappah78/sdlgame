#ifndef LUA_CHUNK_GENERATOR_H_
#define LUA_CHUNK_GENERATOR_H_

class VoxelChunk;
struct lua_State;

class LuaChunkGenerator
{
public:
	LuaChunkGenerator(lua_State* L, int seed);
	~LuaChunkGenerator();
	LuaChunkGenerator(const LuaChunkGenerator& copy) = delete;

	void generateChunk(VoxelChunk* const chunk);

private:
	lua_State* m_L;
	int m_seed;
};

#endif //LUA_CHUNK_GENERATOR_H_
