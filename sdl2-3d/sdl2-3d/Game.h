#ifndef GAME_H_
#define GAME_H_

#include "Engine/Input/Input.h"
#include "Engine/Graphics/Graphics.h"

#include <SDL.h>

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>

#include <lua.hpp>
#include <LuaBridge.h>

class IScreen;

class Game
{
public:
	static Input const input;
	static Graphics const graphics;

	/** Get the currently set lua_State */
	static lua_State* const L() { return m_L; };

	static void setLuaState(lua_State* L)
	{
		m_L = L;
	};

	static void startGameLoop();
	static void shutdownGameLoop();

	static void render(float deltaSec);
	static void resize(int width, int height);
	static void setScreen(IScreen* screen);

	/** Setup a lua state, loading the libaries and fixing the print() */
	static void initLua(lua_State* const L);

private:
	static bool m_running;
	static IScreen* m_currScreen;
	/** A global lua state which might be useful. */
	static lua_State* m_L;

	~Game();
	Game(const Game& copy);
};

#endif //GAME_H_